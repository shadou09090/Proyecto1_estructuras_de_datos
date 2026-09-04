using namespace std;
#include "Juego.h"
#include <algorithm>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>

static int unidadesInicioPorJugador(int numJugadores) {
    if (numJugadores == 3) return 35;
    if (numJugadores == 4) return 30;
    if (numJugadores == 5) return 25;
    if (numJugadores == 6) return 20;
    return -1;
}

static bool esNombreJugadorValido(const string &nombre) {
    return !nombre.empty() && nombre.length() <= 8 && nombre.find(' ') == string::npos;
}

static bool esCodigoTerritorioValido(const string &codigo) {
    size_t punto = codigo.find('.');
    if (punto == string::npos || punto == 0 || punto == codigo.length() - 1) {
        return false;
    }

    try {
        int continente = stoi(codigo.substr(0, punto));
        int numero = stoi(codigo.substr(punto + 1));

        if (continente == 1) return numero >= 1 && numero <= 9;
        if (continente == 2) return numero >= 1 && numero <= 4;
        if (continente == 3) return numero >= 1 && numero <= 7;
        if (continente == 4) return numero >= 1 && numero <= 6;
        if (continente == 5) return numero >= 1 && numero <= 12;
        if (continente == 6) return numero >= 1 && numero <= 4;
    } catch (...) {
        return false;
    }

    return false;
}

static void mostrarErrorFormato(const string &nombreArchivo) {
    cout << "(Archivo sin formato) " << nombreArchivo
         << " no contiene información en el formato esperado." << endl;
}

Territorio* Juego::buscarTerritorio(string codigo) {
    list<list<Territorio> >::iterator itCont;
    for (itCont = Continente.begin(); itCont != Continente.end(); ++itCont) {
        list<Territorio>::iterator itTerr;
        for (itTerr = itCont->begin(); itTerr != itCont->end(); ++itTerr) {
            if (itTerr->getNombre() == codigo) {
                return &(*itTerr);
            }
        }
    }
    return NULL;
}

static bool jugadorControlaContinente(list<Territorio> &continente, const string &color) {
    if (continente.empty()) {
        return false;
    }
    list<Territorio>::iterator it;
    for (it = continente.begin(); it != continente.end(); ++it) {
        if (it->getColorOcupante() != color) {
            return false;
        }
    }
    return true;
}

static int bonusContinente(int indice) {
    switch (indice) {
        case 0: return 5;
        case 1: return 2;
        case 2: return 5;
        case 3: return 3;
        case 4: return 7;
        case 5: return 2;
        default: return 0;
    }
}

static string nombreContinente(int indice) {
    switch (indice) {
        case 0: return "America del Norte";
        case 1: return "America del Sur";
        case 2: return "Europa";
        case 3: return "Africa";
        case 4: return "Asia";
        case 5: return "Oceania";
        default: return "";
    }
}

static void agregarVecino(map<string, vector<string>> &vecinos, const string &a, const string &b) {
    vecinos[a].push_back(b);
    vecinos[b].push_back(a);
}

static const map<string, vector<string>> &obtenerMapaVecinos() {
    static map<string, vector<string>> vecinos;
    static bool inicializado = false;
    if (!inicializado) {
        inicializado = true;

        agregarVecino(vecinos, "1.1", "1.2"); agregarVecino(vecinos, "1.1", "1.6"); agregarVecino(vecinos, "1.1", "5.6");
        agregarVecino(vecinos, "1.2", "1.7"); agregarVecino(vecinos, "1.2", "1.9");
        agregarVecino(vecinos, "1.3", "1.4"); agregarVecino(vecinos, "1.3", "1.9"); agregarVecino(vecinos, "1.3", "2.4");
        agregarVecino(vecinos, "1.4", "1.7"); agregarVecino(vecinos, "1.4", "1.8");
        agregarVecino(vecinos, "1.5", "1.6"); agregarVecino(vecinos, "1.5", "1.8"); agregarVecino(vecinos, "1.5", "3.2");
        agregarVecino(vecinos, "1.6", "1.7"); agregarVecino(vecinos, "1.6", "1.9");
        agregarVecino(vecinos, "1.7", "1.8"); agregarVecino(vecinos, "1.7", "1.9");
        agregarVecino(vecinos, "1.8", "1.9");

        agregarVecino(vecinos, "2.1", "2.2"); agregarVecino(vecinos, "2.1", "2.3");
        agregarVecino(vecinos, "2.2", "2.3"); agregarVecino(vecinos, "2.2", "2.4");
        agregarVecino(vecinos, "2.2", "4.2"); agregarVecino(vecinos, "2.2", "4.5");
        agregarVecino(vecinos, "2.4", "4.5");

        agregarVecino(vecinos, "3.1", "3.2"); agregarVecino(vecinos, "3.1", "3.3"); agregarVecino(vecinos, "3.1", "3.7");
        agregarVecino(vecinos, "3.2", "3.3"); agregarVecino(vecinos, "3.2", "3.4");
        agregarVecino(vecinos, "3.3", "3.6"); agregarVecino(vecinos, "3.3", "3.7");
        agregarVecino(vecinos, "3.4", "3.6"); agregarVecino(vecinos, "3.4", "5.12");
        agregarVecino(vecinos, "3.5", "3.6"); agregarVecino(vecinos, "3.5", "3.7");
        agregarVecino(vecinos, "3.5", "4.3"); agregarVecino(vecinos, "3.5", "4.5"); agregarVecino(vecinos, "3.5", "5.7");
        agregarVecino(vecinos, "3.6", "5.1"); agregarVecino(vecinos, "3.6", "5.11"); agregarVecino(vecinos, "3.6", "5.7");
        agregarVecino(vecinos, "3.7", "4.3"); agregarVecino(vecinos, "3.7", "4.5");

        agregarVecino(vecinos, "4.1", "4.2"); agregarVecino(vecinos, "4.1", "4.4"); agregarVecino(vecinos, "4.1", "4.5"); agregarVecino(vecinos, "4.1", "4.6");
        agregarVecino(vecinos, "4.2", "4.3"); agregarVecino(vecinos, "4.2", "4.4"); agregarVecino(vecinos, "4.2", "4.6"); agregarVecino(vecinos, "4.2", "5.7");
        agregarVecino(vecinos, "4.3", "4.5");
        agregarVecino(vecinos, "4.4", "4.6");

        agregarVecino(vecinos, "5.1", "5.11"); agregarVecino(vecinos, "5.1", "5.2"); agregarVecino(vecinos, "5.1", "5.7");
        agregarVecino(vecinos, "5.2", "5.11"); agregarVecino(vecinos, "5.2", "5.3"); agregarVecino(vecinos, "5.2", "5.8");
        agregarVecino(vecinos, "5.2", "5.9"); agregarVecino(vecinos, "5.2", "5.10");
        agregarVecino(vecinos, "5.3", "5.7"); agregarVecino(vecinos, "5.3", "5.9");
        agregarVecino(vecinos, "5.4", "5.6"); agregarVecino(vecinos, "5.4", "5.8"); agregarVecino(vecinos, "5.4", "5.10"); agregarVecino(vecinos, "5.4", "5.12");
        agregarVecino(vecinos, "5.5", "5.6"); agregarVecino(vecinos, "5.5", "5.8");
        agregarVecino(vecinos, "5.6", "5.8"); agregarVecino(vecinos, "5.6", "5.12");
        agregarVecino(vecinos, "5.8", "5.10");
        agregarVecino(vecinos, "5.9", "6.2");
        agregarVecino(vecinos, "5.10", "5.11"); agregarVecino(vecinos, "5.10", "5.12");

        agregarVecino(vecinos, "6.1", "6.3"); agregarVecino(vecinos, "6.1", "6.4");
        agregarVecino(vecinos, "6.2", "6.3"); agregarVecino(vecinos, "6.2", "6.4");
        agregarVecino(vecinos, "6.3", "6.4");
    }
    return vecinos;
}

static bool sonVecinos(const string &codigoOrigen, const string &codigoDestino) {
    const map<string, vector<string>> &vecinos = obtenerMapaVecinos();
    map<string, vector<string>>::const_iterator it = vecinos.find(codigoOrigen);
    if (it == vecinos.end()) {
        return false;
    }
    vector<string>::const_iterator itVec;
    for (itVec = it->second.begin(); itVec != it->second.end(); ++itVec) {
        if (*itVec == codigoDestino) {
            return true;
        }
    }
    return false;
}

static Jugador *buscarJugadorPorColor(list<Jugador> &jugadores, const string &color) {
    list<Jugador>::iterator it;
    for (it = jugadores.begin(); it != jugadores.end(); ++it) {
        if (it->getColor() == color) {
            return &(*it);
        }
    }
    return NULL;
}

Jugador* Juego::buscarJugador(string nombre) {
    list<Jugador>::iterator it;
    for (it = jugadores.begin(); it != jugadores.end(); ++it) {
        if (it->getNombre() == nombre) {
            return &(*it);
        }
    }
    return NULL;
}

static int lanzarDadoAleatorio() {
    return (rand() % 6) + 1;
}

static vector<int> lanzarDados(int cantidad) {
    vector<int> dados;
    for (int i = 0; i < cantidad; i++) {
        dados.push_back(lanzarDadoAleatorio());
    }
    sort(dados.begin(), dados.end(), greater<int>());
    return dados;
}

static bool leerSiNo(const string &mensaje) {
    cout << mensaje;
    string respuesta;
    getline(cin, respuesta);
    return respuesta == "s" || respuesta == "S" || respuesta == "si" || respuesta == "Si";
}

static void mostrarDados(const vector<int> &dados, const string &etiqueta) {
    cout << "  " << etiqueta << ": ";
    for (size_t i = 0; i < dados.size(); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << dados[i];
    }
    cout << endl;
}


Juego::Juego() {
    juegoIniciado = false;
    juegoTerminado = false;
    std::srand(static_cast<unsigned>(time(nullptr)));
}

int Juego::rand() {
    return lanzarDadoAleatorio();
}

 void Juego::verificarGanador() {
    int vivos = 0;
    string posibleGanador = "";
    list<Jugador>::iterator it;
    for (it = jugadores.begin(); it != jugadores.end(); ++it) {
        if (!it->getestaEliminado()) {
            vivos++;
            posibleGanador = it->getNombre();
        }
    }
    if (vivos == 1) {
        juegoTerminado = true;
    }
}


void Juego::Empezar_a_jugar() {

    cout << "-----------------------BIENVENIDO AL JUEGO DE RISK-----------------------" << endl;
    while (!juegoTerminado) {
        cout << "$Comando: " << endl;
        if (!getline(cin, comando)) {
            break;
        }
        if (comando.empty()) continue;

        string::iterator itCmd;
        for (itCmd = comando.begin(); itCmd != comando.end(); ++itCmd) {
            *itCmd = tolower(*itCmd);
        }
        Gestor_comandos(comando);
    }
}

void Juego::Gestor_comandos(string commando) {

    // obtener_tropas samuel albania

    stringstream ss(commando);
    string accion , parametro1, parametro2;
    ss >> accion; // guarda_la primera palabra en la variable accion

    if (accion == "inicializar") {
        ss >> parametro1;
        Inicializar(parametro1);
    }
    else if (accion == "obtener_unidades") {
        ss >> parametro1;
        Obtener_unidades(parametro1);
    }
    else if (accion == "atacar") {
        ss >> parametro1; // guarda_la segunda palabra en la variable accion
        Atacar(parametro1);
    }
    else if (accion == "fortificar") {
        ss >> parametro1;
        Fortificar(parametro1);
    }
    else if (accion == "estado_juego") {
        Estado_juego();
    }
    else if (accion == "guardar") {
        ss >> parametro1;
        Guardar(parametro1);
    }
    else if (accion == "guardar_comprimido") {
        ss >> parametro1;
        Guardar_comprimido(parametro1);
    }
    else if (accion == "costo_conquista") {
        ss >> parametro1 >> parametro2;
        Costo_conquista(parametro1, parametro2);
    }
    else if (accion == "conquista_mas_barata") {
        ss >> parametro1;
        Conquista_mas_barata(parametro1);
    }
    else if (accion == "ayuda") {
        Ayuda();
    }
    else if (accion == "salir") {
        cout << "Saliendo del juego..." << endl;
        exit(0);
    }
    else if (accion == "lol") {
        LOL2();
    }
    else {
        cout << "Comando no reconocido. Escriba 'ayuda' para ver la lista de comandos." << endl;
    }
}


void Juego::Ayuda() {
    cout << "-----------------------AYUDA-----------------------" << endl;
    cout << "inicializar archivo_inicio.txt" << endl;
    cout << "obtener_unidades nombre_jugador" << endl;
    cout << "atacar nombre_jugador" << endl;
    cout << "fortificar nombre_jugador" << endl;
    cout << "estado_juego" << endl;
    cout << "guardar <nombre_archivo>" << endl;
    cout << "guardar_comprimido <nombre_archivo>" << endl;
    cout << "costo_conquista <nombre_jugador> <territorio>" << endl;
    cout << "conquista_mas_barata <nombre_jugador>" << endl;


}




void Juego::Inicializar(string nombreArchivo) {

    if (juegoIniciado) {
        cout << "(Juego en curso) El juego ya ha sido inicializado." << endl;
        return;
    }

    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        archivo.open("../" + nombreArchivo);
    }

    if (!archivo.is_open()) {
        cout << "(Archivo erróneo) " << nombreArchivo << " no se encuentra o no puede leerse." << endl;
        return;
    }

    if (archivo.peek() == ifstream::traits_type::eof()) {
        cout << "(Archivo vacío) " << nombreArchivo << " no contiene información." << endl;
        archivo.close();
        return;
    }

    jugadores.clear();
    Continente.clear();
    juegoTerminado = false;
    jugadorTurnoActual = "";

    int numJugadores = 0;
    if (!(archivo >> numJugadores) || unidadesInicioPorJugador(numJugadores) == -1) {
        mostrarErrorFormato(nombreArchivo);
        archivo.close();
        return;
    }

    list<string> coloresUsados;

    for (int i = 0; i < numJugadores; i++) {
        string nombre, color;
        if (!(archivo >> nombre >> color)) {
            mostrarErrorFormato(nombreArchivo);
            archivo.close();
            return;
        }

        string::iterator itNombre;
        for (itNombre = nombre.begin(); itNombre != nombre.end(); ++itNombre) {
            *itNombre = tolower(*itNombre);
        }
        string::iterator itColor;
        for (itColor = color.begin(); itColor != color.end(); ++itColor) {
            *itColor = tolower(*itColor);
        }

        if (!esNombreJugadorValido(nombre)) {
            mostrarErrorFormato(nombreArchivo);
            archivo.close();
            return;
        }

        list<string>::const_iterator itColorUsado;
        for (itColorUsado = coloresUsados.begin(); itColorUsado != coloresUsados.end(); ++itColorUsado) {
            if (*itColorUsado == color) {
                mostrarErrorFormato(nombreArchivo);
                archivo.close();
                return;
            }
        }

        coloresUsados.push_back(color);
        jugadores.push_back(Jugador(nombre, color));
    }

    for (int i = 0; i < 6; i++) {
        Continente.push_back(list<Territorio>());
    }

    list<string> territoriosVistos;
    int territoriosLeidos = 0;
    string codigo, colorOcupante;
    int unidades = 0;

    while (archivo >> codigo >> colorOcupante >> unidades) {
        string::iterator itColorOcupante;
        for (itColorOcupante = colorOcupante.begin(); itColorOcupante != colorOcupante.end(); ++itColorOcupante) {
            *itColorOcupante = tolower(*itColorOcupante);
        }

        if (!esCodigoTerritorioValido(codigo) || unidades <= 0) {
            mostrarErrorFormato(nombreArchivo);
            archivo.close();
            return;
        }

        list<string>::const_iterator itTerritorioVisto;
        for (itTerritorioVisto = territoriosVistos.begin(); itTerritorioVisto != territoriosVistos.end(); ++itTerritorioVisto) {
            if (*itTerritorioVisto == codigo) {
                mostrarErrorFormato(nombreArchivo);
                archivo.close();
                return;
            }
        }
        territoriosVistos.push_back(codigo);

        size_t punto = codigo.find('.');
        int indiceContinente = stoi(codigo.substr(0, punto)) - 1;

        bool colorValido = false;
        list<Jugador>::iterator itJugador;
        for (itJugador = jugadores.begin(); itJugador != jugadores.end(); ++itJugador) {
            if (itJugador->getColor() == colorOcupante) {
                colorValido = true;
                itJugador->agregarTerritorio(codigo);
                itJugador->setnumUnidadesActuales(itJugador->getnumUnidadesActuales() + unidades);
                break;
            }
        }

        if (!colorValido) {
            mostrarErrorFormato(nombreArchivo);
            archivo.close();
            return;
        }

        Territorio territorio(codigo);
        territorio.setColorOcupante(colorOcupante);
        territorio.setNumUnidades(unidades);
        territorio.setNumero(stoi(codigo.substr(punto + 1)));

        list<list<Territorio>>::iterator itContinente = Continente.begin();
        advance(itContinente, indiceContinente);
        itContinente->push_back(territorio);

        territoriosLeidos++;
    }

    if (territoriosLeidos != 42) {
        mostrarErrorFormato(nombreArchivo);
        archivo.close();
        return;
    }

    string datoExtra;
    if (archivo >> datoExtra) {
        mostrarErrorFormato(nombreArchivo);
        archivo.close();
        return;
    }

    int unidadesEsperadas = unidadesInicioPorJugador(numJugadores);
    list<Jugador>::iterator itJug;
    for (itJug = jugadores.begin(); itJug != jugadores.end(); ++itJug) {
        if (itJug->getnumUnidadesActuales() != unidadesEsperadas) {
            mostrarErrorFormato(nombreArchivo);
            archivo.close();
            return;
        }
    }

    if (!jugadores.empty()) {
        jugadorTurnoActual = jugadores.front().getNombre();
    }

    juegoIniciado = true;
    cout << "(Comando correcto) El juego se ha inicializado correctamente." << endl;

    archivo.close();
}

void Juego::Obtener_unidades(string nombre_jugador) {

    if (!juegoIniciado) {
        cout << "(Juego no inicializado) Esta partida no ha sido inicializada correctamente." << endl;
        return;
    }


    if (juegoTerminado) {
        cout << "(Juego terminado) Esta partida ya tuvo un ganador." << endl;
        return;
    }


    Jugador* jugadorEncontrado = nullptr;
    list<Jugador>::iterator itJugador;
    for (itJugador = jugadores.begin(); itJugador != jugadores.end(); ++itJugador) {
        if (itJugador->getNombre() == nombre_jugador) {
            jugadorEncontrado = &(*itJugador);
            break;
        }
    }

    if (jugadorEncontrado == nullptr) {
        cout << "(Jugador no vAlido) El jugador " << nombre_jugador << " no forma parte de esta partida." << endl;
        return;
    }


    if (jugadorTurnoActual != nombre_jugador) {
        cout << "(Jugador fuera de turno) No es el turno del jugador " << nombre_jugador << " ." << endl;
        return;
    }

    string colorJugador = jugadorEncontrado->getColor();
    int numTerritorios = jugadorEncontrado->getCantidadTerritorios();
    int unidadesPorTerritorios = numTerritorios / 3;
    int unidadesPorContinentes = 0;

    cout << endl;
    cout << "========== OBTENCION DE UNIDADES ==========" << endl;
    cout << "Jugador: " << nombre_jugador << endl;
    cout << endl;

    int indiceContinente = 0;
    list<list<Territorio> >::iterator itCont;
    for (itCont = Continente.begin(); itCont != Continente.end(); ++itCont) {
        if (jugadorControlaContinente(*itCont, colorJugador)) {
            int bonus = bonusContinente(indiceContinente);
            unidadesPorContinentes += bonus;
            cout << "  Continente controlado: " << nombreContinente(indiceContinente)
                 << " (+" << bonus << " unidades)" << endl;
        }
        indiceContinente++;
    }

    int unidadesDisponibles = unidadesPorTerritorios + unidadesPorContinentes;

    cout << endl;
    cout << "Resumen de unidades adicionales:" << endl;
    cout << "  - Por territorios (" << numTerritorios << " / 3): "
         << unidadesPorTerritorios << " unidades" << endl;
    cout << "  - Por continentes: " << unidadesPorContinentes << " unidades" << endl;
    cout << "  - Total a reclamar: " << unidadesDisponibles << " unidades" << endl;
    cout << endl;

    cout << "Territorios del jugador:" << endl;
    int numeroTerritorio = 1;
    list<list<Territorio> >::iterator itContTerr;
    for (itContTerr = Continente.begin(); itContTerr != Continente.end(); ++itContTerr) {
        list<Territorio>::iterator itTerr;
        for (itTerr = itContTerr->begin(); itTerr != itContTerr->end(); ++itTerr) {
            if (itTerr->getColorOcupante() == colorJugador) {
                cout << "  " << numeroTerritorio << ". " << itTerr->getNombre()
                     << " - " << itTerr->getNumUnidades() << " unidades" << endl;
                numeroTerritorio++;
            }
        }
    }
    cout << endl;

    if (unidadesDisponibles == 0) {
        jugadorEncontrado->setobtuvoUnidades(true);
        cout << "(Comando correcto) El jugador " << nombre_jugador
             << " ha terminado de reclamar y ubicar sus unidades." << endl;
        return;
    }

    int unidadesRestantes = unidadesDisponibles;
    while (unidadesRestantes > 0) {
        cout << "Unidades restantes por asignar: " << unidadesRestantes << endl;
        cout << "Ingrese el codigo del territorio: ";
        string codigoTerritorio;
        getline(cin, codigoTerritorio);

        Territorio *territorio = buscarTerritorio( codigoTerritorio);
        if (territorio == nullptr) {
            cout << "Codigo de territorio invalido. Intente nuevamente." << endl;
            continue;
        }

        if (territorio->getColorOcupante() != colorJugador) {
            cout << "El territorio " << codigoTerritorio << " no pertenece al jugador "
                 << nombre_jugador << ". Intente nuevamente." << endl;
            continue;
        }

        cout << "Ingrese la cantidad de unidades para " << codigoTerritorio << ": ";
        string entradaCantidad;
        getline(cin, entradaCantidad);

        int cantidad = 0;
        try {
            cantidad = stoi(entradaCantidad);
        } catch (...) {
            cout << "Cantidad invalida. Intente nuevamente." << endl;
            continue;
        }

        if (cantidad <= 0 || cantidad > unidadesRestantes) {
            cout << "La cantidad debe estar entre 1 y " << unidadesRestantes
                 << ". Intente nuevamente." << endl;
            continue;
        }

        territorio->setNumUnidades(territorio->getNumUnidades() + cantidad);
        jugadorEncontrado->setnumUnidadesActuales(
            jugadorEncontrado->getnumUnidadesActuales() + cantidad);
        unidadesRestantes -= cantidad;

        cout << "Se asignaron " << cantidad << " unidades a " << codigoTerritorio
             << ". Nuevo total en territorio: " << territorio->getNumUnidades() << endl;
        cout << endl;
    }

    jugadorEncontrado->setobtuvoUnidades(true);
    cout << "(Comando correcto) El jugador " << nombre_jugador
         << " ha terminado de reclamar y ubicar sus unidades." << endl;
}



void Juego::Atacar(string nombre_jugador) {
    if (!juegoIniciado) {
        cout << "(Juego no inicializado) Esta partida no ha sido inicializada correctamente." << endl;
        return;
    }

    if (juegoTerminado) {
        cout << "(Juego terminado) Esta partida ya tuvo un ganador." << endl;
        return;
    }

    Jugador *jugadorAtacante = nullptr;
    list<Jugador>::iterator itJugador;
    for (itJugador = jugadores.begin(); itJugador != jugadores.end(); ++itJugador) {
        if (itJugador->getNombre() == nombre_jugador) {
            jugadorAtacante = &(*itJugador);
            break;
        }
    }

    if (jugadorAtacante == nullptr) {
        cout << "(Jugador no vAlido) El jugador " << nombre_jugador << " no forma parte de esta partida." << endl;
        return;
    }

    if (jugadorTurnoActual != nombre_jugador) {
        cout << "(Jugador fuera de turno) No es el turno del jugador " << nombre_jugador << " ." << endl;
        return;
    }

    if (!jugadorAtacante->getobtuvoUnidades()) {
        cout << "(Jugador no ha ubicado unidades) El jugador " << nombre_jugador
             << " no ha ejecutado el comando obtener_unidades." << endl;
        return;
    }

    string colorAtacante = jugadorAtacante->getColor();

    cout << endl;
    cout << "========== FASE DE ATAQUE ==========" << endl;
    cout << "Jugador: " << nombre_jugador << endl;
    cout << "Ingrese 'fin' como territorio de origen para terminar la fase de ataque." << endl;
    cout << endl;

    while (!juegoTerminado) {
        cout << "Territorio de origen (desde donde atacar): ";
        string codigoOrigen;
        getline(cin, codigoOrigen);

        if (codigoOrigen == "fin") {
            break;
        }

        Territorio *territorioOrigen = buscarTerritorio( codigoOrigen);
        if (territorioOrigen == nullptr) {
            cout << "Codigo de territorio invalido. Intente nuevamente." << endl;
            continue;
        }

        if (territorioOrigen->getColorOcupante() != colorAtacante) {
            cout << "El territorio " << codigoOrigen << " no pertenece al jugador "
                 << nombre_jugador << ". Intente nuevamente." << endl;
            continue;
        }

        if (territorioOrigen->getNumUnidades() <= 1) {
            cout << "El territorio " << codigoOrigen
                 << " debe tener mas de 1 unidad para atacar. Intente nuevamente." << endl;
            continue;
        }

        cout << "Territorio de destino (a atacar): ";
        string codigoDestino;
        getline(cin, codigoDestino);

        Territorio *territorioDestino = buscarTerritorio( codigoDestino);
        if (territorioDestino == nullptr) {
            cout << "Codigo de territorio invalido. Intente nuevamente." << endl;
            continue;
        }

        if (!sonVecinos(codigoOrigen, codigoDestino)) {
            cout << "El territorio " << codigoDestino << " no es vecino de " << codigoOrigen
                 << ". Intente nuevamente." << endl;
            continue;
        }

        if (territorioDestino->getColorOcupante() == colorAtacante) {
            cout << "No puede atacar un territorio propio. Intente nuevamente." << endl;
            continue;
        }

        if (territorioDestino->getNumUnidades() <= 0) {
            cout << "El territorio " << codigoDestino << " no tiene unidades para defender. Intente nuevamente." << endl;
            continue;
        }

        string colorDefensor = territorioDestino->getColorOcupante();
        Jugador *jugadorDefensor = buscarJugadorPorColor(jugadores, colorDefensor);

        cout << endl;
        cout << "--- Combate: " << codigoOrigen << " (" << territorioOrigen->getNumUnidades()
             << " unidades) vs " << codigoDestino << " (" << territorioDestino->getNumUnidades()
             << " unidades) ---" << endl;

        bool continuarCombate = true;
        while (continuarCombate && territorioOrigen->getNumUnidades() > 1
               && territorioDestino->getNumUnidades() > 0 && !juegoTerminado) {

            int dadosAtacante = min(3, territorioOrigen->getNumUnidades() - 1);
            int dadosDefensor = min(2, territorioDestino->getNumUnidades());
            int comparaciones = min(dadosAtacante, dadosDefensor);

            vector<int> valoresAtacante = lanzarDados(dadosAtacante);
            vector<int> valoresDefensor = lanzarDados(dadosDefensor);

            cout << endl;
            cout << "Resultado de los dados:" << endl;
            mostrarDados(valoresAtacante, "Dados atacante (rojos)");
            mostrarDados(valoresDefensor, "Dados defensor (blancos)");

            int perdidasAtacante = 0;
            int perdidasDefensor = 0;

            for (int i = 0; i < comparaciones; i++) {
                if (valoresAtacante[i] > valoresDefensor[i]) {
                    perdidasDefensor++;
                } else {
                    perdidasAtacante++;
                }
            }

            territorioOrigen->setNumUnidades(territorioOrigen->getNumUnidades() - perdidasAtacante);
            territorioDestino->setNumUnidades(territorioDestino->getNumUnidades() - perdidasDefensor);
            jugadorAtacante->setnumUnidadesActuales(
                jugadorAtacante->getnumUnidadesActuales() - perdidasAtacante);
            if (jugadorDefensor != nullptr) {
                jugadorDefensor->setnumUnidadesActuales(
                    jugadorDefensor->getnumUnidadesActuales() - perdidasDefensor);
            }

            cout << "Perdidas en esta ronda:" << endl;
            cout << "  Atacante (" << codigoOrigen << "): -" << perdidasAtacante << " unidades" << endl;
            cout << "  Defensor (" << codigoDestino << "): -" << perdidasDefensor << " unidades" << endl;
            cout << "Unidades restantes:" << endl;
            cout << "  " << codigoOrigen << ": " << territorioOrigen->getNumUnidades() << endl;
            cout << "  " << codigoDestino << ": " << territorioDestino->getNumUnidades() << endl;

            if (territorioDestino->getNumUnidades() == 0) {
                cout << endl;
                cout << "El territorio " << codigoDestino << " ha sido conquistado!" << endl;

                int maximoMovible = territorioOrigen->getNumUnidades() - 1;
                int unidadesAMover = 0;
                bool movimientoValido = false;

                while (!movimientoValido) {
                    cout << "Ingrese cuantas unidades mover a " << codigoDestino
                         << " (minimo 1, maximo " << maximoMovible << "): ";
                    string entradaMovimiento;
                    getline(cin, entradaMovimiento);

                    try {
                        unidadesAMover = stoi(entradaMovimiento);
                    } catch (...) {
                        cout << "Cantidad invalida. Intente nuevamente." << endl;
                        continue;
                    }

                    if (unidadesAMover < 1 || unidadesAMover > maximoMovible) {
                        cout << "La cantidad debe estar entre 1 y " << maximoMovible
                             << ". Intente nuevamente." << endl;
                        continue;
                    }
                    movimientoValido = true;
                }

                territorioOrigen->setNumUnidades(territorioOrigen->getNumUnidades() - unidadesAMover);
                territorioDestino->setNumUnidades(unidadesAMover);
                territorioDestino->setColorOcupante(colorAtacante);

                jugadorAtacante->agregarTerritorio(codigoDestino);
                if (jugadorDefensor != nullptr) {
                    jugadorDefensor->eliminarTerritorio(codigoDestino);
                }
                if (jugadorDefensor->getestaEliminado()) {
                    cout << "El jugador " << jugadorDefensor->getNombre() << " ha sido eliminado." << endl;

                }

                cout << "Se movieron " << unidadesAMover << " unidades a " << codigoDestino << "." << endl;
                verificarGanador();
                if (juegoTerminado) {
                    cout << "(Juego terminado) El jugador " << nombre_jugador << " ha ganado la partida." << endl;
                    return;
                }
                continuarCombate = false;
            } else if (territorioOrigen->getNumUnidades() <= 1) {
                cout << "El territorio " << codigoOrigen << " no puede continuar atacando." << endl;
                continuarCombate = false;
            } else {
                continuarCombate = leerSiNo("Desea continuar atacando? (s/n): ");
            }
        }

        cout << endl;
        if (juegoTerminado) {
            break;
        }
    }

    jugadorAtacante->setyaAtaque(true);
    cout << "(Comando correcto) El jugador " << nombre_jugador << " ha terminado de atacar." << endl;
}

void Juego::Fortificar(string nombre_jugador) {

    if (!juegoIniciado) {
        cout << "(Juego no inicializado) Esta partida no ha sido inicializada correctamente." << endl;
        return;
    }

    if (juegoTerminado) {
        cout << "(Juego terminado) Esta partida ya tuvo un ganador." << endl;
        return;
    }

    Jugador* j = buscarJugador(nombre_jugador);



    if (j == NULL) {
        cout << "(Jugador no vAlido) El jugador " << nombre_jugador << " no forma parte de esta partida." << endl;
        return;
    }

    if (nombre_jugador != jugadorTurnoActual) {
        cout << "(Jugador fuera de turno) No es el turno del jugador " << nombre_jugador << " ." << endl;
        return;
    }

    if (!j->getYaAtaque()) {
        cout << "(Jugador no ha atacado) El jugador " << nombre_jugador << " no ha ejecutado el comando atacar." << endl;
        return;
    }


    string origen, destino;
    int unidades;

    cout << "Ingrese el territorio origen: ";
    cin >> origen;
    cout << "Ingrese el territorio vecino destino: ";
    cin >> destino;
    cout << "Ingrese la cantidad de unidades a trasladar: ";
    cin >> unidades;
    cin.ignore();
    Territorio* tOrigen = buscarTerritorio(origen);
    Territorio* tDestino = buscarTerritorio(destino);

    if (tOrigen == NULL ) {
        cout << "Origem no existe" << endl;
        return;
    }
    if (tDestino == NULL) {
        cout << "Destino no existe" << endl;
        return;
    }
    if (!j->poseeTerritorio(origen)) {

        cout << "El jugador no posee el territorio origen" << endl;
        return;
    }
    if (!j->poseeTerritorio(destino)) {
        cout << "El jugador no posee el territorio destino" << endl;
        return;
    }
    if (unidades <= 0) {
        cout << "No pueden ser 0 unidades return" << endl;
        return;
    }
    if (tOrigen->getNumUnidades() - unidades < 1) {
        cout << "Hay un minimo de 1 unidad que debe quedarse " << endl;
        return;
    }
    if (!sonVecinos(origen, destino)) {
        cout << "Movimiento invalido: los territorios no son vecinos." << endl;
        return;
    }
    tOrigen->setNumUnidades(tOrigen->getNumUnidades() - unidades);
    tDestino->setNumUnidades(tDestino->getNumUnidades() + unidades);

    j->setYaFortifique(true);
    cout << "(Comando correcto) El jugador " << nombre_jugador
         << " ha terminado de fortificar su posiciOn." << endl;

    //cout<<"Ya se fotifico este jugador"<<endl;
    j->reiniciarTurno();

    verificarGanador();
    if (juegoTerminado) {
        cout << "(Juego terminado) El ganador es " << nombre_jugador << "." << endl;
        return;
    }

    list<Jugador>::iterator itJ;
    for (itJ = jugadores.begin(); itJ != jugadores.end(); ++itJ) {
        if (itJ->getNombre() == jugadorTurnoActual) {
            do {
                ++itJ;
                if (itJ == jugadores.end()) {
                    itJ = jugadores.begin();
                }
            } while (itJ->getestaEliminado());

            jugadorTurnoActual = itJ->getNombre();
            break;
        }
    }
}

void Juego::Estado_juego() {

    if (!juegoIniciado) {
        cout << "(Juego no inicializado) Esta partida no ha sido inicializada correctamente." << endl;
        return;
    }

    if (juegoTerminado) {
        cout << "(Juego terminado) Esta partida ya tuvo un ganador." << endl;
        return;
    }

    cout << endl;
    cout << "========== ESTADO DEL JUEGO ==========" << endl;
    cout << endl;

    cout << "Numero de jugadores: " << jugadores.size() << endl;
    cout << endl;

    cout << "Jugadores:" << endl;
    int numeroJugador = 1;
    list<Jugador>::iterator itJugador;
    for (itJugador = jugadores.begin(); itJugador != jugadores.end(); ++itJugador) {
        cout << "  " << numeroJugador << ". " << itJugador->getNombre()
             << " - Color: " << itJugador->getColor()
             << " - Territorios: " << itJugador->getCantidadTerritorios()
             << " - Unidades: " << itJugador->getnumUnidadesActuales();
        if (itJugador->getestaEliminado()) {
            cout << " (eliminado)";
        }
        cout << endl;
        numeroJugador++;
    }
    cout << endl;

    cout << "Turno actual: " << jugadorTurnoActual << endl;
    cout << endl;

    cout << "Territorios:" << endl;
    int indiceContinente = 0;
    list<list<Territorio> >::iterator itCont;
    for (itCont = Continente.begin(); itCont != Continente.end(); ++itCont) {
        cout << "  " << nombreContinente(indiceContinente) << ":" << endl;
        list<Territorio>::iterator itTerr;
        for (itTerr = itCont->begin(); itTerr != itCont->end(); ++itTerr) {
            cout << "    " << itTerr->getNombre()
                 << " - Color: " << itTerr->getColorOcupante()
                 << " - Unidades: " << itTerr->getNumUnidades() << endl;
        }
        indiceContinente++;
    }
    cout << endl;
}

void Juego::Guardar(string nombre_archivo) {

    if (!juegoIniciado) {
        cout << "(Juego no inicializado) Esta partida no ha sido inicializada correctamente." << endl;
        return;
    }

    ofstream archivo(nombre_archivo);
    if (!archivo.is_open()) {
        cout << "(Error al guardar) La partida no ha sido guardada correctamente." << endl;
        return;
    }
    archivo << jugadores.size() << "\n";
    list<Jugador>::iterator itJ;
    for (itJ = jugadores.begin(); itJ != jugadores.end(); ++itJ) {
        archivo << itJ->getNombre() << " " << itJ->getColor() << "\n";
    }
    list<list<Territorio> >::iterator itC;
    for (itC = Continente.begin(); itC != Continente.end(); ++itC) {
        list<Territorio>::iterator itT;
        for (itT = itC->begin(); itT != itC->end(); ++itT) {
            archivo << itT->getNombre() << " "
                    << itT->getColorOcupante() << " "
                    << itT->getNumUnidades() << "\n";
        }
    }

    archivo.close();

    cout << "(Comando correcto) La partida ha sido guardada correctamente." << endl;
}

//IGUAL NOS PIDEN GRAFOS PA SEMANA MAS ADELANTE
void Juego::Guardar_comprimido(string nombre_archivo) {

    if (!juegoIniciado) {
        cout << "(Juego no inicializado) Esta partida no ha sido inicializada correctamente." << endl;
        return;
    }

    ofstream archivo(nombre_archivo);
    if (!archivo.is_open()) {
        cout << "(Error al codificar y/o guardar) La partida no ha sido codificada ni guardada correctamente." << endl;
        return;
    }

    archivo.close();

    cout << "(Comando correcto) La partida ha sido codificada y guardada correctamente." << endl;
}
//NIKOLAS ESTO ES EN SEAMANAS PROXIMAS
void Juego::Costo_conquista(string nombre_jugador, string territorio) {
    if (!juegoIniciado) {
        cout << "Esta partida no ha sido inicializada correctamente." << endl;
        return;
    }

    if (juegoTerminado) {
        cout << "Esta partida ya tuvo un ganador." << endl;
        return;
    }

    Jugador* j = buscarJugador(nombre_jugador);
    if (j == NULL) {
        cout << "El jugador " << nombre_jugador << " no forma parte de esta partida." << endl;
        return;
    }

    Territorio* t = buscarTerritorio(territorio);
    if (t == NULL) {
        cout << "El territorio " << territorio << " no existe." << endl;
        return;
    }

    string territorioInicio = "";
    list<string> territoriosIntermedios;
    int unidades = 0;

    cout << "Para conquistar el territorio " << territorio << ", " << nombre_jugador
         << " debe atacar desde " << territorioInicio << ", pasando por los territorios ";

    bool primero = true;
    list<string>::iterator it;
    for (it = territoriosIntermedios.begin(); it != territoriosIntermedios.end(); ++it) {
        if (!primero) {
            cout << ", ";
        }
        cout << *it;
        primero = false;
    }

    cout << ". Debe conquistar " << unidades << " unidades de ejercito." << endl;
}

void Juego::Conquista_mas_barata(string nombre_jugador) {
    if (!juegoIniciado) {
        cout << "Esta partida no ha sido inicializada correctamente." << endl;
        return;
    }

    if (juegoTerminado) {
        cout << "Esta partida ya tuvo un ganador." << endl;
        return;
    }

    Jugador* j = buscarJugador(nombre_jugador);
    if (j == NULL) {
        cout << "El jugador " << nombre_jugador << " no forma parte de esta partida." << endl;
        return;
    }

    string territorio1 = "";
    string territorio2 = "";
    list<string> territoriosIntermedios;
    int unidades = 0;

    cout << "La conquista mas barata es avanzar sobre el territorio " << territorio1
         << " desde el territorio " << territorio2 << ". Para conquistar el territorio " << territorio1
         << ", debe atacar desde " << territorio2 << ", pasando por los territorios ";

    bool primero = true;
    list<string>::iterator it;
    for (it = territoriosIntermedios.begin(); it != territoriosIntermedios.end(); ++it) {
        if (!primero) {
            cout << ", ";
        }
        cout << *it;
        primero = false;
    }

    cout << ". Debe conquistar " << unidades << " unidades de ejercito." << endl;
}

void Juego::LOL2() {




    for (int i = 0; i < 10; i++) {
        cout << R"(+---------------------------------------+
+                                       +
+  -----------------------------------  +
+ < Soy capaz de hacer el codigo! >     +
+  -----------------------------------  +
+        \   ^__^                       +
+         \  (oo)\_______               +
+            (__)\       )\/\           +
+                ||----w |              +
+                ||     ||              +
+---------------------------------------+
)";
    }

}

