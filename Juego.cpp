#include "Juego.h"
#include "Huffman.h"
#include "Archivos.h"

using namespace std;

static int enteroEstricto(const string& texto) {
    size_t usados = 0;
    int valor = stoi(texto, &usados);
    if (usados != texto.size()) throw invalid_argument("Cantidad invalida");
    return valor;
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

        agregarVecino(vecinos, "1.1", "1.2");
        agregarVecino(vecinos, "1.1", "1.6");
        agregarVecino(vecinos, "1.1", "5.6");
        agregarVecino(vecinos, "1.2", "1.6");
        agregarVecino(vecinos, "1.2", "1.7");
        agregarVecino(vecinos, "1.2", "1.9");
        agregarVecino(vecinos, "1.3", "1.4");
        agregarVecino(vecinos, "1.3", "1.9");
        agregarVecino(vecinos, "1.3", "2.4");
        agregarVecino(vecinos, "1.4", "1.7");
        agregarVecino(vecinos, "1.4", "1.8");
        agregarVecino(vecinos, "1.4", "1.9");
        agregarVecino(vecinos, "1.5", "1.6");
        agregarVecino(vecinos, "1.5", "1.7");
        agregarVecino(vecinos, "1.5", "1.8");
        agregarVecino(vecinos, "1.5", "3.2");
        agregarVecino(vecinos, "1.6", "1.7");
        agregarVecino(vecinos, "1.7", "1.8");
        agregarVecino(vecinos, "1.7", "1.9");
        agregarVecino(vecinos, "2.1", "2.2");
        agregarVecino(vecinos, "2.1", "2.3");
        agregarVecino(vecinos, "2.2", "2.3");
        agregarVecino(vecinos, "2.2", "2.4");
        agregarVecino(vecinos, "2.2", "4.5");
        agregarVecino(vecinos, "2.3", "2.4");
        agregarVecino(vecinos, "3.1", "3.2");
        agregarVecino(vecinos, "3.1", "3.3");
        agregarVecino(vecinos, "3.1", "3.4");
        agregarVecino(vecinos, "3.1", "3.7");
        agregarVecino(vecinos, "3.2", "3.4");
        agregarVecino(vecinos, "3.3", "3.4");
        agregarVecino(vecinos, "3.3", "3.5");
        agregarVecino(vecinos, "3.3", "3.6");
        agregarVecino(vecinos, "3.3", "3.7");
        agregarVecino(vecinos, "3.4", "3.6");
        agregarVecino(vecinos, "3.5", "3.6");
        agregarVecino(vecinos, "3.5", "3.7");
        agregarVecino(vecinos, "3.5", "4.3");
        agregarVecino(vecinos, "3.5", "4.5");
        agregarVecino(vecinos, "3.5", "5.7");
        agregarVecino(vecinos, "3.6", "5.1");
        agregarVecino(vecinos, "3.6", "5.7");
        agregarVecino(vecinos, "3.6", "5.11");
        agregarVecino(vecinos, "3.7", "4.5");
        agregarVecino(vecinos, "4.1", "4.2");
        agregarVecino(vecinos, "4.1", "4.5");
        agregarVecino(vecinos, "4.1", "4.6");
        agregarVecino(vecinos, "4.2", "4.3");
        agregarVecino(vecinos, "4.2", "4.4");
        agregarVecino(vecinos, "4.2", "4.5");
        agregarVecino(vecinos, "4.2", "4.6");
        agregarVecino(vecinos, "4.2", "5.7");
        agregarVecino(vecinos, "4.3", "4.5");
        agregarVecino(vecinos, "4.3", "5.7");
        agregarVecino(vecinos, "4.4", "4.6");
        agregarVecino(vecinos, "5.1", "5.2");
        agregarVecino(vecinos, "5.1", "5.3");
        agregarVecino(vecinos, "5.1", "5.7");
        agregarVecino(vecinos, "5.1", "5.11");
        agregarVecino(vecinos, "5.2", "5.3");
        agregarVecino(vecinos, "5.2", "5.8");
        agregarVecino(vecinos, "5.2", "5.9");
        agregarVecino(vecinos, "5.2", "5.10");
        agregarVecino(vecinos, "5.2", "5.11");
        agregarVecino(vecinos, "5.3", "5.7");
        agregarVecino(vecinos, "5.3", "5.9");
        agregarVecino(vecinos, "5.4", "5.6");
        agregarVecino(vecinos, "5.4", "5.8");
        agregarVecino(vecinos, "5.4", "5.10");
        agregarVecino(vecinos, "5.4", "5.12");
        agregarVecino(vecinos, "5.5", "5.6");
        agregarVecino(vecinos, "5.5", "5.8");
        agregarVecino(vecinos, "5.6", "5.8");
        agregarVecino(vecinos, "5.6", "5.12");
        agregarVecino(vecinos, "5.8", "5.10");
        agregarVecino(vecinos, "5.9", "6.2");
        agregarVecino(vecinos, "5.10", "5.11");
        agregarVecino(vecinos, "5.10", "5.12");
        agregarVecino(vecinos, "6.1", "6.3");
        agregarVecino(vecinos, "6.1", "6.4");
        agregarVecino(vecinos, "6.2", "6.3");
        agregarVecino(vecinos, "6.2", "6.4");
        agregarVecino(vecinos, "6.3", "6.4");
    }
    return vecinos;
}

void Juego::configurarVecinos() {
    const map<string, vector<string>>& vecinos = obtenerMapaVecinos();
    for (list<Territorio>& continente : Continente) {
        for (Territorio& territorio : continente) {
            map<string, vector<string>>::const_iterator it = vecinos.find(territorio.getNombre());
            if (it == vecinos.end()) continue;
            for (const string& vecino : it->second) territorio.agregarVecino(vecino);
        }
    }
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
        string candidato = it->getNombre();
        for (char& c : candidato) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
        for (char& c : nombre) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
        if (candidato == nombre) {
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
    for (;;) {
        cout << mensaje;
        string linea, respuesta, extra;
        if (!getline(cin, linea)) return false;
        istringstream entrada(linea);
        entrada >> respuesta;
        for (char& c : respuesta) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
        if (!(entrada >> extra)) {
            if (respuesta == "s" || respuesta == "si") return true;
            if (respuesta == "n" || respuesta == "no") return false;
        }
        cout << "Respuesta invalida. Escriba s o n.\n";
    }
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
    static bool semillaInicializada = false;
    if (!semillaInicializada) { srand(static_cast<unsigned>(time(nullptr))); semillaInicializada = true; }
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
    cout << "-----------------------BIENVENIDO AL JUEGO DE RISK-----------------------\n";
    while (!salirSolicitado) {
        cout << "$ " << flush;
        if (!getline(cin, comando)) break;
        if (!comando.empty()) Gestor_comandos(comando);
    }
}

static const map<string, string>& usos() {
    static const map<string, string> tabla = {
        {"inicializar", "inicializar <archivo.txt|archivo.bin>"},
        {"obtener_unidades", "obtener_unidades <nombre_jugador>"},
        {"atacar", "atacar <nombre_jugador>"}, {"fortificar", "fortificar <nombre_jugador>"},
        {"estado_juego", "estado_juego"}, {"guardar", "guardar <nombre_archivo>"},
        {"guardar_comprimido", "guardar_comprimido <nombre_archivo.bin>"},
        {"costo_conquista", "costo_conquista <nombre_jugador> <codigo_territorio>"},
        {"conquista_mas_barata", "conquista_mas_barata <nombre_jugador>"},
        {"ayuda", "ayuda [comando]"}, {"salir", "salir"}
    };
    return tabla;
}
void Juego::Gestor_comandos(string commando) {
    istringstream entrada(commando);
    string accion, argumento;
    entrada >> accion;
    if (accion.empty() || accion[0] == '#') return;
    for (char& c : accion) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    if (accion == "guardar_juego") accion = "guardar";
    vector<string> parametros;
    while (entrada >> ws && entrada.peek() != char_traits<char>::eof()) {
        if (entrada.peek() == '"') {
            entrada.get();
            if (!getline(entrada, argumento, '"') || entrada.eof()) {
                cout << "(Comando invalido) Comillas sin cerrar.\n"; return;
            }
        } else entrada >> argumento;
        parametros.push_back(argumento);
    }
    map<string, string>::const_iterator uso = usos().find(accion);
    if (uso == usos().end()) { cout << "Comando no reconocido. Escriba 'ayuda'.\n"; return; }
    size_t esperados = (accion == "estado_juego" || accion == "salir") ? 0 : (accion == "costo_conquista" ? 2 : 1);
    if ((accion == "ayuda" ? parametros.size() > 1 : parametros.size() != esperados) ||
        any_of(parametros.begin(), parametros.end(), [](const string& p){return p.empty();})) {
        cout << "(Comando invalido) Uso: " << uso->second << '\n'; return;
    }
    if (accion == "ayuda") {
        if (parametros.empty()) Ayuda();
        else {
            for (char& c : parametros[0]) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
            if (usos().count(parametros[0])) cout << "Uso: " << usos().at(parametros[0]) << '\n';
            else cout << "Comando no reconocido.\n";
        }
        return;
    }
    if (accion == "salir") { salirSolicitado = true; return; }
    if (accion == "estado_juego") { Estado_juego(); return; }
    if (accion == "inicializar") { Inicializar(parametros[0]); return; }
    if (accion == "guardar") { Guardar(parametros[0]); return; }
    if (accion == "guardar_comprimido") { Guardar_comprimido(parametros[0]); return; }
    for (char& c : parametros[0]) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    Jugador* seleccionado = buscarJugador(parametros[0]);
    if (seleccionado) parametros[0] = seleccionado->getNombre();
    if (accion == "obtener_unidades") Obtener_unidades(parametros[0]);
    else if (accion == "atacar") Atacar(parametros[0]);
    else if (accion == "fortificar") Fortificar(parametros[0]);
    else if (accion == "costo_conquista") Costo_conquista(parametros[0], parametros[1]);
    else if (accion == "conquista_mas_barata") Conquista_mas_barata(parametros[0]);
}

void Juego::Ayuda() {
    cout << "-----------------------AYUDA-----------------------\n";
    Jugador* actual = buscarJugador(jugadorTurnoActual);
    for (const pair<const string, string>& uso : usos()) {
        const string& accion = uso.first;
        bool disponible = accion == "ayuda" || accion == "salir";
        if (!juegoIniciado) disponible = disponible || accion == "inicializar";
        else {
            disponible = disponible || accion == "guardar" || accion == "guardar_comprimido";
            if (!juegoTerminado) {
                disponible = disponible || accion == "estado_juego" || accion == "costo_conquista" || accion == "conquista_mas_barata";
                if (actual) {
                    if (!actual->getobtuvoUnidades()) disponible = disponible || accion == "obtener_unidades";
                    else if (!actual->getYaAtaque()) disponible = disponible || accion == "atacar";
                    else disponible = disponible || accion == "fortificar";
                }
            }
        }
        if (disponible) cout << uso.second << '\n';
    }
    if (juegoIniciado && !juegoTerminado) cout << "Turno actual: " << jugadorTurnoActual << '\n';
    cout << "Use ayuda <comando> para consultar cualquier sintaxis y comillas para archivos con espacios.\n";
    if (juegoIniciado) cout << "En atacar y fortificar, 'fin' termina la fase. Conserve el auxiliar .estado de cada partida.\n";
}

void Juego::Inicializar(string nombreArchivo) {
    if (juegoIniciado) { cout << "(Juego en curso) El juego ya ha sido inicializado.\n"; return; }
    ifstream archivo(nombreArchivo, ios::binary);
    string rutaArchivo = nombreArchivo;
    if (!archivo) {
        archivo.clear(); rutaArchivo = "../" + nombreArchivo;
        archivo.open(rutaArchivo, ios::binary);
    }
    if (!archivo) { cout << "(Archivo erróneo) " << nombreArchivo << " no se encuentra o no puede leerse.\n"; return; }
    if (archivo.peek() == char_traits<char>::eof()) {
        cout << "(Archivo vacío) " << nombreArchivo << " no contiene información.\n"; return;
    }
    try {
        archivo.seekg(0, ios::end);
        if (archivo.tellg() > 4 * 1024 * 1024) throw runtime_error("Archivo demasiado grande");
        archivo.seekg(0);
        string texto((istreambuf_iterator<char>(archivo)), istreambuf_iterator<char>());
        bool binario = texto.size() >= 2 && (static_cast<unsigned char>(texto[1]) <= 1);
        if (binario) { istringstream comprimido(texto, ios::binary); texto = Huffman::descomprimir(comprimido); }
        if (Archivos::existe(rutaArchivo + ".estado")) {
            if (!Archivos::esArchivo(rutaArchivo + ".estado")) throw runtime_error("Auxiliar invalido");
            ifstream auxiliar(rutaArchivo + ".estado", ios::binary);
            if (!auxiliar) throw runtime_error("No se pudo leer el auxiliar");
            auxiliar.seekg(0, ios::end);
            if (auxiliar.tellg() > 4 * 1024 * 1024) throw runtime_error("Estado demasiado grande");
            auxiliar.seekg(0);
            string estado((istreambuf_iterator<char>(auxiliar)), istreambuf_iterator<char>());
            Juego candidato, tablero;
            if (!candidato.cargarTexto(estado) || !tablero.cargarTexto(texto, false) ||
                candidato.serializarTablero() != tablero.serializarTablero())
                throw runtime_error("El archivo auxiliar no corresponde a esta partida");
            texto = estado;
        }
        if (!cargarTexto(texto)) {
            Juego recuperada;
            if (!recuperada.cargarTexto(texto, false)) throw runtime_error("Formato invalido");
            if (!leerSiNo("Los totales no corresponden a un inicio. Es una partida guardada que desea recuperar? (s/n): "))
                throw runtime_error("Totales iniciales invalidos");
            *this = move(recuperada);
            cout << "Tablero recuperado sin auxiliar: turno de " << jugadorTurnoActual
                 << ", fases reiniciadas, manos vacias y contador de canjes en cero.\n";
        }
        cout << "(Comando correcto) El juego se ha inicializado correctamente.\n";
    } catch (const exception&) { mostrarErrorFormato(nombreArchivo); }
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
        cout << "(Jugador no válido) El jugador " << nombre_jugador << " no forma parte de esta partida." << endl;
        return;
    }


    if (jugadorTurnoActual != nombre_jugador) {
        cout << "(Jugador fuera de turno) No es el turno del jugador " << nombre_jugador << " ." << endl;
        return;
    }

    if (jugadorEncontrado->getobtuvoUnidades()) {
        cout << "(Fase completada) Ya obtuvo unidades en este turno." << endl;
        return;
    }
    Juego respaldo = *this;
    int unidadesPorCartas = canjearCartas(*jugadorEncontrado);
    if (!cin) { *this = respaldo; return; }
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

    int unidadesDisponibles = unidadesPorTerritorios + unidadesPorContinentes + unidadesPorCartas;
    if (unidadesDisponibles > numeric_limits<int>::max() - jugadorEncontrado->getnumUnidadesActuales()) {
        cout << "La cantidad de unidades excede el limite admitido." << endl;
        *this = respaldo;
        return;
    }

    cout << endl;
    cout << "Resumen de unidades adicionales:" << endl;
    cout << "  - Por territorios (" << numTerritorios << " / 3): "
         << unidadesPorTerritorios << " unidades" << endl;
    cout << "  - Por continentes: " << unidadesPorContinentes << " unidades" << endl;
    cout << "  - Por canjes: " << unidadesPorCartas << " unidades" << endl;
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
        if (!getline(cin, codigoTerritorio)) { *this = respaldo; return; }

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
        if (!getline(cin, entradaCantidad)) { *this = respaldo; return; }

        int cantidad = 0;
        try {
            cantidad = enteroEstricto(entradaCantidad);
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
        cout << "(Jugador no válido) El jugador " << nombre_jugador << " no forma parte de esta partida." << endl;
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

    if (jugadorAtacante->getYaAtaque()) {
        cout << "(Fase completada) Ya termino de atacar en este turno.\n"; return;
    }
    Juego respaldo = *this;
    bool conquisto = false;
    string colorAtacante = jugadorAtacante->getColor();

    cout << endl;
    cout << "========== FASE DE ATAQUE ==========" << endl;
    cout << "Jugador: " << nombre_jugador << endl;
    cout << "Ingrese 'fin' como territorio de origen para terminar la fase de ataque." << endl;
    cout << endl;

    while (!juegoTerminado) {
        cout << "Territorio de origen (desde donde atacar): ";
        string codigoOrigen;
        if (!getline(cin, codigoOrigen)) { *this = respaldo; return; }

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
        if (!getline(cin, codigoDestino)) { *this = respaldo; return; }

        Territorio *territorioDestino = buscarTerritorio( codigoDestino);
        if (territorioDestino == nullptr) {
            cout << "Codigo de territorio invalido. Intente nuevamente." << endl;
            continue;
        }

        if (!territorioOrigen->esVecino(codigoDestino)) {
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
                        if (!getline(cin, entradaMovimiento)) { *this = respaldo; return; }

                        try {
                        unidadesAMover = enteroEstricto(entradaMovimiento);
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

                conquisto = true;
                jugadorAtacante->agregarTerritorio(codigoDestino);
                if (jugadorDefensor != nullptr) {
                    jugadorDefensor->eliminarTerritorio(codigoDestino);
                }
                if (jugadorDefensor && jugadorDefensor->getestaEliminado()) {
                    manos[nombre_jugador].splice(manos[nombre_jugador].end(), manos[jugadorDefensor->getNombre()]);
                    cout << "El jugador " << jugadorDefensor->getNombre() << " ha sido eliminado." << endl;

                }

                cout << "Se movieron " << unidadesAMover << " unidades a " << codigoDestino << "." << endl;
                verificarGanador();
                if (juegoTerminado) {
                    entregarCarta(nombre_jugador);
                    jugadorAtacante->setyaAtaque(true);
                    cout << "(Juego terminado) El jugador " << nombre_jugador << " ha ganado la partida." << endl;
                    return;
                }
                continuarCombate = false;
            } else if (territorioOrigen->getNumUnidades() <= 1) {
                cout << "El territorio " << codigoOrigen << " no puede continuar atacando." << endl;
                continuarCombate = false;
            } else {
                continuarCombate = leerSiNo("Desea continuar atacando? (s/n): ");
                if (!cin) { *this = respaldo; return; }
            }
        }

        cout << endl;
        if (juegoTerminado) {
            break;
        }
    }

    if (conquisto) entregarCarta(nombre_jugador);
    jugadorAtacante->setyaAtaque(true);
    cout << "(Comando correcto) El jugador " << nombre_jugador << " ha terminado de atacar." << endl;
}

void Juego::Fortificar(string nombre_jugador) {
    if (!juegoIniciado) { cout << "(Juego no inicializado) Esta partida no ha sido inicializada correctamente.\n"; return; }
    if (juegoTerminado) { cout << "(Juego terminado) Esta partida ya tuvo un ganador.\n"; return; }
    Jugador* j = buscarJugador(nombre_jugador);
    if (!j) { cout << "(Jugador no válido) El jugador " << nombre_jugador << " no forma parte de esta partida.\n"; return; }
    if (nombre_jugador != jugadorTurnoActual) { cout << "(Jugador fuera de turno) No es el turno del jugador " << nombre_jugador << " .\n"; return; }
    if (!j->getYaAtaque()) { cout << "(Jugador no ha atacado) El jugador " << nombre_jugador << " no ha ejecutado el comando atacar.\n"; return; }
    string origen, destino, cantidad;
    cout << "Ingrese el territorio origen ('fin' para terminar sin mover): ";
    if (!getline(cin, origen)) return;
    if (origen != "fin") {
        cout << "Ingrese el territorio vecino destino: ";
        if (!getline(cin, destino)) return;
        cout << "Ingrese la cantidad de unidades a trasladar: ";
        if (!getline(cin, cantidad)) return;
        int unidades;
        try { unidades = enteroEstricto(cantidad); } catch (const exception&) { cout << "Cantidad invalida.\n"; return; }
        Territorio* a = buscarTerritorio(origen);
        Territorio* b = buscarTerritorio(destino);
        if (!a || !b || !j->poseeTerritorio(origen) || !j->poseeTerritorio(destino) ||
            !a->esVecino(destino) || unidades <= 0 || unidades >= a->getNumUnidades()) {
            cout << "Movimiento invalido: use territorios propios vecinos y conserve una unidad en origen.\n"; return;
        }
        a->setNumUnidades(a->getNumUnidades() - unidades);
        b->setNumUnidades(b->getNumUnidades() + unidades);
    }
    j->reiniciarTurno();
    siguienteTurno();
    cout << "(Comando correcto) El jugador " << nombre_jugador << " ha terminado de fortificar su posición.\n";
}
void Juego::siguienteTurno() {
    list<Jugador>::iterator it = find_if(jugadores.begin(), jugadores.end(), [&](Jugador& j){return j.getNombre() == jugadorTurnoActual;});
    if (it == jugadores.end()) return;
    for (size_t i = 0; i < jugadores.size(); ++i) {
        if (++it == jugadores.end()) it = jugadores.begin();
        if (!it->getestaEliminado()) { jugadorTurnoActual = it->getNombre(); return; }
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
    if (!juegoIniciado) { cout << "(Juego no inicializado) Esta partida no ha sido inicializada correctamente.\n"; return; }
    try {
        guardarArchivos(nombre_archivo, false);
        cout << "(Comando correcto) La partida ha sido guardada correctamente.\n";
        cout << "Conserve tambien " << nombre_archivo << ".estado para recuperar turno y cartas.\n";
    } catch (const exception&) {
        cout << "(Error al guardar) La partida no ha sido guardada correctamente.\n";
    }
}

void Juego::Guardar_comprimido(string nombre_archivo) {
    if (!juegoIniciado) { cout << "(Juego no inicializado) Esta partida no ha sido inicializada correctamente.\n"; return; }
    try {
        string extension = nombre_archivo.size() >= 4 ? nombre_archivo.substr(nombre_archivo.size()-4) : "";
        for (char& c : extension) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
        if (extension != ".bin") nombre_archivo += ".bin";
        guardarArchivos(nombre_archivo, true);
        cout << "(Comando correcto) La partida ha sido codificada y guardada correctamente.\n";
        cout << "Conserve tambien " << nombre_archivo << ".estado para recuperar turno y cartas.\n";
    } catch (const exception&) {
        cout << "(Error al codificar y/o guardar) La partida no ha sido codificada ni guardada correctamente.\n";
    }
}

struct Rutas {
    map<string, long long> costo;
    map<string, string> anterior;
};
static Rutas calcularRutas(Juego& juego, Jugador& jugador) {
    Rutas r;
    using Paso = pair<long long, string>;
    priority_queue<Paso, vector<Paso>, greater<Paso>> cola;
    for (list<Territorio>& continente : juego.Continente) for (Territorio& t : continente) {
        bool propio = t.getColorOcupante() == jugador.getColor();
        r.costo[t.getNombre()] = propio ? 0 : numeric_limits<long long>::max();
        if (propio) cola.push({0, t.getNombre()});
    }
    while (!cola.empty()) {
        Paso paso = cola.top(); cola.pop();
        if (paso.first != r.costo[paso.second]) continue;
        for (const string& vecino : obtenerMapaVecinos().at(paso.second)) {
            Territorio* t = juego.buscarTerritorio(vecino);
            long long costo = paso.first + (t->getColorOcupante() == jugador.getColor() ? 0 : t->getNumUnidades());
            if (costo < r.costo[vecino]) {
                r.costo[vecino] = costo; r.anterior[vecino] = paso.second;
                cola.push({costo, vecino});
            }
        }
    }
    return r;
}
static vector<string> rutaHasta(const Rutas& r, string destino) {
    vector<string> camino{destino};
    while (r.anterior.count(destino)) { destino = r.anterior.at(destino); camino.push_back(destino); }
    reverse(camino.begin(), camino.end());
    return camino;
}
static bool validarConsulta(Juego& juego, const string& nombre) {
    if (!juego.juegoIniciado) { cout << "(Juego no inicializado) Esta partida no ha sido inicializada correctamente.\n"; return false; }
    if (juego.juegoTerminado) { cout << "(Juego terminado) Esta partida ya tuvo un ganador.\n"; return false; }
    Jugador* j = juego.buscarJugador(nombre);
    if (!j) { cout << "(Jugador no válido) El jugador " << nombre << " no forma parte de esta partida.\n"; return false; }
    if (j->getestaEliminado()) { cout << "(Jugador eliminado) No controla territorios.\n"; return false; }
    return true;
}
static void imprimirRuta(const Rutas& r, const vector<string>& camino, const string& nombre) {
    cout << "Para conquistar el territorio " << camino.back() << ", " << nombre
         << " debe atacar desde " << camino.front() << ", pasando por los territorios ";
    for (size_t i = 1; i < camino.size(); ++i) cout << (i > 1 ? ", " : "") << camino[i];
    cout << ". Debe conquistar " << r.costo.at(camino.back()) << " unidades de ejército.\n";
}
void Juego::Costo_conquista(string nombre_jugador, string territorio) {
    if (!validarConsulta(*this, nombre_jugador)) return;
    Jugador* j = buscarJugador(nombre_jugador);
    Territorio* t = buscarTerritorio(territorio);
    if (!t) { cout << "(Territorio no valido) El territorio " << territorio << " no existe.\n"; return; }
    if (j->poseeTerritorio(territorio)) { cout << "(Comando correcto) El jugador ya controla " << territorio << ". Costo: 0 unidades.\n"; return; }
    Rutas r = calcularRutas(*this, *j);
    if (r.costo.at(territorio) == numeric_limits<long long>::max()) { cout << "No existe una ruta de conquista.\n"; return; }
    cout << "(Comando correcto) "; imprimirRuta(r, rutaHasta(r, territorio), nombre_jugador);
}
void Juego::Conquista_mas_barata(string nombre_jugador) {
    if (!validarConsulta(*this, nombre_jugador)) return;
    Jugador* j = buscarJugador(nombre_jugador);
    Rutas r = calcularRutas(*this, *j);
    string destino;
    long long menor = numeric_limits<long long>::max();
    for (const pair<const string, long long>& par : r.costo) if (!j->poseeTerritorio(par.first) && par.second < menor) {
        menor = par.second; destino = par.first;
    }
    if (destino.empty()) { cout << "No hay territorios enemigos alcanzables.\n"; return; }
    vector<string> camino = rutaHasta(r, destino);
    cout << "(Comando correcto) La conquista más barata es avanzar sobre el territorio " << destino
         << " desde el territorio " << camino.front() << ". ";
    imprimirRuta(r, camino, nombre_jugador);
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

