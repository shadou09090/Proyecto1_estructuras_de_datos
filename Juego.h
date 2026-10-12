
#ifndef PROYECTO_ESTRUCTURAS_PAIS_JUEGO_H
#define PROYECTO_ESTRUCTURAS_PAIS_JUEGO_H
#include <list>
#include "Territorio.h"
#include "Jugador.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cctype>
#include <iterator>
#include <random>
#include <algorithm>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

using namespace std;


class Juego {
    public:
    string comando;
    bool juegoIniciado;
    bool juegoTerminado;
    string jugadorTurnoActual;
    list<Jugador> jugadores;
    list<list<Territorio>> Continente;

public:
    Juego();
    list<int> mazo;
    map<string, list<int>> manos;
    int canjes = 0;
    bool salirSolicitado = false;
    string serializar();
    string serializarTablero();
    void guardarArchivos(const string& nombreArchivo, bool comprimido);
    bool cargarTexto(const string& texto, bool validarInicio = true);
    void prepararCartas();
    int canjearCartas(Jugador& jugador);
    void entregarCarta(const string& jugador);
    void siguienteTurno();
    void configurarVecinos();

    void verificarGanador();

    void Empezar_a_jugar();

    void Ayuda();

    void LOL2();

    void Gestor_comandos(string commando);

    void Inicializar(string nombreArchivo);

    void Obtener_unidades(string nombre_jugador);

    void Atacar(string nombre_jugador);

    void Estado_juego();
    void Fortificar(string nombre_jugador);

    void Guardar(string nombre_archivo);
    void Guardar_comprimido(string nombre_archivo);
    void Costo_conquista(string nombre_jugador, string territorio);
    void Conquista_mas_barata(string nombre_jugador);



    Jugador* buscarJugador(string nombre);
    Territorio* buscarTerritorio(string codigoONombre);

    int rand();
};


#endif
