//
// Created by gniko on 29/08/2026.
//
using namespace std;
#include "Jugador.h"
#include <iostream>
Jugador::Jugador() {
    this->nombre = "";
    this->color = "";
    this->numUnidadesActuales = 0;
    this->obtuvoUnidades = false;
    this->estaEliminado = false;
    this->yaAtaque = false;
}


Jugador::Jugador(string nombre, string color) {
    this->nombre = nombre;
    this->color = color;
    this->numUnidadesActuales = 0;
    this->obtuvoUnidades = false;
    this->estaEliminado = false;
    this->yaAtaque = false;
}
string Jugador::getNombre() {
    return nombre;
}

bool Jugador::getYaAtaque() {
    return yaAtaque;
}
void Jugador::setNombre(string nombre) {
    this->nombre = nombre;
}
void Jugador::setColor(string color) {
    this->color = color;
}

string Jugador::getColor() {
    return color;
}
int Jugador::getnumUnidadesActuales() {
    return numUnidadesActuales;
}
void Jugador::setnumUnidadesActuales(int numUnidadesActuales) {
    this->numUnidadesActuales = numUnidadesActuales;
}
bool Jugador::getobtuvoUnidades() {
    return obtuvoUnidades;
}
void Jugador::setobtuvoUnidades(bool obtuvoUnidades) {
    this->obtuvoUnidades = obtuvoUnidades;
}

void Jugador::setyaAtaque(bool yaAtaque ) {
    this->yaAtaque = yaAtaque;
}
bool Jugador::getestaEliminado() {
    return estaEliminado;
}
void Jugador::setestaEliminado(bool estaEliminado) {
    this->estaEliminado = estaEliminado;
}
void Jugador::reiniciarTurno() {
    this->yaAtaque = false;
    this->obtuvoUnidades = false;
    this->yaFortifique = false;
    cout << "(Jugador) Se ha reiniciado el turno del jugador: " << this->nombre << endl;
}
int Jugador::getCantidadTerritorios() {
    return this->nombresTerritoriosOcupados.size();
}
void Jugador::agregarTerritorio(string territorio) {
    this->nombresTerritoriosOcupados.push_back(territorio);
}
bool Jugador::eliminarTerritorio(string territorio) {
    list<string>::iterator it;
    for (it = this->nombresTerritoriosOcupados.begin(); it != this->nombresTerritoriosOcupados.end(); ++it) {
        if (*it == territorio) {
            this->nombresTerritoriosOcupados.erase(it);
            if (this->nombresTerritoriosOcupados.empty()) {
                this->estaEliminado = true;
            }
            return true;
        }
    }
    return false;


}

bool Jugador::getYaFortifique() {
    return yaFortifique;
}

void Jugador::setYaFortifique(bool yaFortifique) {
    this->yaFortifique = yaFortifique;
}

bool Jugador::poseeTerritorio(string territorio) {
    list<string>::iterator it;
    for (it = nombresTerritoriosOcupados.begin(); it != nombresTerritoriosOcupados.end(); ++it) {
        if (*it == territorio) {
            return true;
        }
    }
    return false;
}






