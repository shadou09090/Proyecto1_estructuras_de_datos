//
// Created by gniko on 29/08/2026.
//

#include "Territorio.h"

#include <list>

Territorio::Territorio(string nombre) {
    this->nombre = nombre;
    this->numero = 0;
    this->numUnidades = 0;
}
string Territorio::getNombre() {
    return nombre;
}
int Territorio::getNumero() {
    return numero;
}

void Territorio::setNumero(int numero) {
    this->numero = numero;
}
void Territorio::setNombre(string nombre) {
    this->nombre = nombre;
}

string Territorio::getColorOcupante() {
    return colorOcupante;
}

void Territorio::setColorOcupante(string colorOcupante) {
    this->colorOcupante = colorOcupante;
}

int Territorio::getNumUnidades() {
    return numUnidades;
}

void Territorio::setNumUnidades(int numUnidades) {
    this->numUnidades = numUnidades;
}

void Territorio::agregarVecino(string vecino) {
    territoriosVecinos.push_back(vecino);
}

bool Territorio::esVecino(string vecino) {
    pmr::list<string>::iterator it;
    for (it = territoriosVecinos.begin(); it != territoriosVecinos.end(); ++it) {
        if (*it == vecino) {
            return true;
        }
    }
    return false;
}