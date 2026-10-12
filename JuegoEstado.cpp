
#include "Juego.h"
#include "Huffman.h"
#include <functional>
#include "Archivos.h"
#include <chrono>

using namespace std;

namespace {
const int cantidades[] = {9, 4, 7, 6, 12, 4};
string codigoCarta(int id) {
    if (id >= 42) return "comodin";
    for (int c = 0; c < 6; ++c) {
        if (id < cantidades[c]) return to_string(c + 1) + "." + to_string(id + 1);
        id -= cantidades[c];
    }
    return "";
}
int tipoCarta(int id) { return id >= 42 ? 3 : id % 3; }
bool grupoValido(int a, int b, int c) {
    int tipos[] = {tipoCarta(a), tipoCarta(b), tipoCarta(c)};
    int comodines = 0;
    set<int> distintos;
    for (int t : tipos) { if (t == 3) ++comodines; else distintos.insert(t); }
    return distintos.size() == 1 || distintos.size() + comodines == 3;
}
void minusculas(string& s) {
    for (char& c : s) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
}
bool finalLinea(istringstream& linea) { linea >> ws; return linea.eof(); }
}

void Juego::prepararCartas() {
    vector<int> cartas;
    for (int i = 0; i < 44; ++i) cartas.push_back(i);
    static mt19937 generador(random_device{}());
    shuffle(cartas.begin(), cartas.end(), generador);
    mazo.assign(cartas.begin(), cartas.end());
    manos.clear();
    for (Jugador& j : jugadores) manos[j.getNombre()] = {};
    canjes = 0;
}

void Juego::entregarCarta(const string& jugador) {
    if (mazo.empty()) { cout << "No quedan cartas en el mazo.\n"; return; }
    int carta = mazo.front(); mazo.pop_front();
    manos[jugador].push_back(carta);
    cout << "Carta obtenida: " << carta + 1 << " (" << codigoCarta(carta) << ").\n";
}

int Juego::canjearCartas(Jugador& jugador) {
    list<int>& mano = manos[jugador.getNombre()];
    int refuerzos = 0;
    const char* dibujos[] = {"infanteria", "caballeria", "artilleria", "comodin"};
    while (mano.size() >= 3) {
        cout << "Cartas de " << jugador.getNombre() << ":\n";
        for (int id : mano) cout << "  " << id + 1 << ": " << codigoCarta(id) << " - " << dibujos[tipoCarta(id)] << '\n';
        cout << "Ingrese tres numeros de carta para canjear, o 'fin' para continuar: ";
        string respuesta;
        if (!getline(cin, respuesta) || respuesta == "fin") break;
        istringstream entrada(respuesta);
        int a, b, c;
        if (!(entrada >> a >> b >> c) || !finalLinea(entrada) ||
            a < 1 || b < 1 || c < 1 || a > 44 || b > 44 || c > 44) {
            cout << "Seleccione tres cartas validas.\n"; continue;
        }
        --a; --b; --c;
        if (a == b || a == c || b == c || find(mano.begin(), mano.end(), a) == mano.end() ||
            find(mano.begin(), mano.end(), b) == mano.end() || find(mano.begin(), mano.end(), c) == mano.end() ||
            !grupoValido(a, b, c)) { cout << "Combinacion de cartas invalida.\n"; continue; }
        int premio = canjes < 5 ? 4 + 2 * canjes : 15 + 5 * (canjes - 5);
        if (canjes >= 1000000 || static_cast<long long>(jugador.getnumUnidadesActuales()) + refuerzos + premio + 6 > numeric_limits<int>::max()) {
            cout << "El canje excede el limite de unidades admitido.\n"; break;
        }
        ++canjes;
        refuerzos += premio;
        for (int id : {a, b, c}) {
            Territorio* territorio = buscarTerritorio(codigoCarta(id));
            if (territorio && territorio->getColorOcupante() == jugador.getColor()) {
                territorio->setNumUnidades(territorio->getNumUnidades() + 2);
                jugador.setnumUnidadesActuales(jugador.getnumUnidadesActuales() + 2);
                cout << "Bonus de carta: 2 unidades en " << territorio->getNombre() << ".\n";
            }
            mano.remove(id); mazo.push_back(id);
        }
        cout << "Canje " << canjes << ": " << premio << " unidades adicionales.\n";
    }
    return refuerzos;
}

string Juego::serializarTablero() {
    ostringstream salida;
    salida << jugadores.size() << '\n';
    for (Jugador& j : jugadores) salida << j.getNombre() << ' ' << j.getColor() << '\n';
    for (list<Territorio>& continente : Continente) for (Territorio& t : continente)
        salida << t.getNombre() << ' ' << t.getColorOcupante() << ' ' << t.getNumUnidades() << '\n';
    return salida.str();
}

string Juego::serializar() {
    ostringstream salida;
    salida << serializarTablero();
    salida << "RISK_ESTADO_V1 " << jugadorTurnoActual << ' ' << juegoTerminado << ' ' << canjes << '\n';
    for (Jugador& j : jugadores) {
        salida << j.getNombre() << ' ' << j.getobtuvoUnidades() << ' ' << j.getYaAtaque() << ' ' << manos[j.getNombre()].size();
        for (int id : manos[j.getNombre()]) salida << ' ' << id;
        salida << '\n';
    }
    salida << mazo.size();
    for (int id : mazo) salida << ' ' << id;
    salida << '\n';
    return salida.str();
}

void Juego::guardarArchivos(const string& nombreArchivo, bool comprimido) {

    ostringstream contenido(ios::out | ios::binary);
    if (comprimido) Huffman::comprimir(serializarTablero(), contenido);
    else contenido << serializarTablero();
    const string datos[] = {contenido.str(), serializar()};
    const string nombres[] = {nombreArchivo, nombreArchivo + ".estado"};

    string sufijo = ".risk-" + to_string(chrono::steady_clock::now().time_since_epoch().count());
    string temporales[2], respaldos[2];
    bool respaldado[] = {false, false}, instalado[] = {false, false};
    for (int i = 0; i < 2; ++i) {
        temporales[i] = nombres[i] + sufijo + ".tmp";
        respaldos[i] = nombres[i] + sufijo + ".bak";
        if ((Archivos::existe(nombres[i]) && !Archivos::esArchivo(nombres[i])) ||
            Archivos::existe(temporales[i]) || Archivos::existe(respaldos[i]))
            throw runtime_error("Destino de guardado invalido");
    }
    try {
        for (int i = 0; i < 2; ++i) {
            ofstream archivo(temporales[i], ios::binary);
            if (!archivo) throw runtime_error("No se pudo abrir el archivo temporal");
            archivo << datos[i]; archivo.close();
            if (!archivo) throw runtime_error("No se pudo escribir la partida");
        }
        for (int i = 0; i < 2; ++i) {
            if (Archivos::existe(nombres[i])) {
                Archivos::renombrar(nombres[i], respaldos[i]); respaldado[i] = true;
            }
            Archivos::renombrar(temporales[i], nombres[i]); instalado[i] = true;
        }
    } catch (const exception&) {
        for (int i = 1; i >= 0; --i) {
            if (instalado[i]) Archivos::intentarEliminar(nombres[i]);
            if (respaldado[i]) Archivos::intentarRenombrar(respaldos[i], nombres[i]);
            Archivos::intentarEliminar(temporales[i]);
        }
        throw;
    }
    for (int i = 0; i < 2; ++i) {
        if (respaldado[i]) Archivos::intentarEliminar(respaldos[i]);
    }
}

bool Juego::cargarTexto(const string& texto, bool validarInicio) {
    Juego nuevo;
    istringstream entrada(texto);
    string linea, extra;
    int n;
    if (!getline(entrada, linea)) return false;
    if (linea.compare(0, 3, "\xEF\xBB\xBF") == 0) linea.erase(0, 3);
    istringstream cabecera(linea);
    if (!(cabecera >> n) || !finalLinea(cabecera) || n < 3 || n > 6) return false;
    set<string> nombres, colores;
    const set<string> permitidos = {"verde", "azul", "rojo", "amarillo", "negro", "gris"};
    for (int i = 0; i < n; ++i) {
        if (!getline(entrada, linea)) return false;
        istringstream datos(linea);
        string nombre, color;
        if (!(datos >> nombre >> color) || !finalLinea(datos)) return false;
        minusculas(nombre); minusculas(color);
        if (nombre.empty() || nombre.size() > 8 || !permitidos.count(color) ||
            !nombres.insert(nombre).second || !colores.insert(color).second) return false;
        nuevo.jugadores.emplace_back(nombre, color);
    }
    set<string> pendientes;
    for (int c = 0; c < 6; ++c) {
        nuevo.Continente.emplace_back();
        for (int t = 1; t <= cantidades[c]; ++t) pendientes.insert(to_string(c + 1) + "." + to_string(t));
    }
    for (int i = 0; i < 42; ++i) {
        if (!getline(entrada, linea)) return false;
        istringstream datos(linea);
        string codigo, color;
        long long unidades;
        if (!(datos >> codigo >> color >> unidades) || !finalLinea(datos) ||
            !pendientes.erase(codigo) || unidades <= 0 || unidades > numeric_limits<int>::max()) return false;
        minusculas(color);
        list<Jugador>::iterator j = find_if(nuevo.jugadores.begin(), nuevo.jugadores.end(), [&](Jugador& jugador){return jugador.getColor() == color;});
        if (j == nuevo.jugadores.end() || unidades > numeric_limits<int>::max() - j->getnumUnidadesActuales()) return false;
        j->agregarTerritorio(codigo);
        j->setnumUnidadesActuales(j->getnumUnidadesActuales() + static_cast<int>(unidades));
        Territorio t(codigo);
        t.setNumero(stoi(codigo.substr(2))); t.setColorOcupante(color); t.setNumUnidades(static_cast<int>(unidades));
        list<list<Territorio>>::iterator continente = nuevo.Continente.begin(); advance(continente, codigo[0] - '1');
        continente->push_back(t);
    }
    nuevo.jugadorTurnoActual = nuevo.jugadores.front().getNombre();
    for (Jugador& j : nuevo.jugadores) j.setestaEliminado(j.getCantidadTerritorios() == 0);
    nuevo.prepararCartas();
    entrada >> ws;
    if (entrada.eof()) {
        const int iniciales[] = {35, 30, 25, 20};
        if (validarInicio) {
            for (Jugador& j : nuevo.jugadores) if (j.getnumUnidadesActuales() != iniciales[n - 3]) return false;
        } else {
            for (Jugador& j : nuevo.jugadores) if (!j.getestaEliminado()) {
                nuevo.jugadorTurnoActual = j.getNombre(); break;
            }
            nuevo.verificarGanador();
        }
    } else {
        string marca;
        int terminado;
        if (!(entrada >> marca >> nuevo.jugadorTurnoActual >> terminado >> nuevo.canjes) ||
            marca != "RISK_ESTADO_V1" || (terminado != 0 && terminado != 1) ||
            nuevo.canjes < 0 || nuevo.canjes > 1000000) return false;
        Jugador* actual = nuevo.buscarJugador(nuevo.jugadorTurnoActual);
        if (!actual || actual->getestaEliminado()) return false;
        nuevo.juegoTerminado = terminado;
        set<string> vistos;
        set<int> cartas;
        nuevo.manos.clear(); nuevo.mazo.clear();
        function<bool(list<int>&, int)> leerCartas = [&](list<int>& destino, int total) {
            if (total < 0 || total > 44) return false;
            for (int i = 0, id; i < total; ++i) {
                if (!(entrada >> id) || id < 0 || id >= 44 || !cartas.insert(id).second) return false;
                destino.push_back(id);
            }
            return true;
        };
        for (int i = 0; i < n; ++i) {
            string nombre;
            int obtuvo, ataco, total;
            if (!(entrada >> nombre >> obtuvo >> ataco >> total)) return false;
            Jugador* j = nuevo.buscarJugador(nombre);
            if (!j || !vistos.insert(nombre).second || obtuvo < 0 || obtuvo > 1 || ataco < 0 || ataco > obtuvo ||
                (nombre != nuevo.jugadorTurnoActual && (obtuvo || ataco))) return false;
            j->setobtuvoUnidades(obtuvo); j->setyaAtaque(ataco);
            if (!leerCartas(nuevo.manos[nombre], total) || (j->getestaEliminado() && total)) return false;
        }
        int total;
        if (!(entrada >> total) || !leerCartas(nuevo.mazo, total) || cartas.size() != 44 || (entrada >> extra)) return false;
        int vivos = 0;
        for (Jugador& j : nuevo.jugadores) if (!j.getestaEliminado()) ++vivos;
        if (nuevo.juegoTerminado != (vivos == 1)) return false;
    }
    nuevo.configurarVecinos();
    nuevo.juegoIniciado = true;
    *this = move(nuevo);
    return true;
}
