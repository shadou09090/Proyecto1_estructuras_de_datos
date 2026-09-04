//
// Created by gniko on 29/08/2026.
//

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
    //void obtener_unidades(nombre_jugador)
    void jugarTurno();

    void verificarGanador();

    void Empezar_a_jugar();

    void Ayuda();

    void LOL2();

    void Gestor_comandos(string commando);

    void Inicializar(string nombreArchivo);

    void Obtener_unidades(string nombre_jugador);

    void Atacar(string nombre_jugador);

    void Estado_juego();
//
    void Fortificar(string nombre_jugador);

//
    void Guardar(string nombre_archivo);
//
    void Guardar_comprimido(string nombre_archivo);
//
    void Costo_conquista(string nombre_jugador, string territorio);
//
    void Conquista_mas_barata(string nombre_jugador);



    Jugador* buscarJugador(string nombre);
    Territorio* buscarTerritorio(string codigoONombre);

    int rand();
};


#endif //PROYECTO_ESTRUCTURAS_PAIS_JUEGO_H