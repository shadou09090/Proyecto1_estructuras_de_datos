//
// Created by gniko on 29/08/2026.
//

#ifndef PROYECTO_ESTRUCTURAS_PAIS_JUGADOR_H
#define PROYECTO_ESTRUCTURAS_PAIS_JUGADOR_H
#include <string>
#include <list>
//#include "Carta.h"
using namespace std;
class Jugador {
private:
private:
    string nombre;
    string color;
    int numUnidadesActuales;
    bool estaEliminado;
    bool obtuvoUnidades;
    bool yaAtaque;
    list<string> nombresTerritoriosOcupados;
    //list<Carta> manoCartas;
    bool yaFortifique;
public:
    Jugador();
    Jugador(string nombre, string color);
    string getNombre();
    void setNombre(string nombre);

    string getColor();
    void setColor(string color);

    int getnumUnidadesActuales();
    void setnumUnidadesActuales(int numUnidadesActuales);

    bool getobtuvoUnidades();
    void setobtuvoUnidades(bool obtuvoUnidades);

    bool getYaAtaque();
    void setyaAtaque(bool yaAtaque);
    bool getestaEliminado();
    void setestaEliminado(bool estaEliminado);

    void reiniciarTurno();

    int getCantidadTerritorios();
    void agregarTerritorio(string territorio);
    bool eliminarTerritorio(string territorio);
    bool getYaFortifique();
    void setYaFortifique(bool yaFortifique);
    bool poseeTerritorio(string territorio);
};
#endif //PROYECTO_ESTRUCTURAS_PAIS_JUGADOR_H