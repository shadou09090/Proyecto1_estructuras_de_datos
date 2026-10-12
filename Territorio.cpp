

#include "Territorio.h"

#include <list>

using namespace std;

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
    if (vecino != nombre && !esVecino(vecino)) territoriosVecinos.push_back(vecino);
}

bool Territorio::esVecino(string vecino) {
    list<string>::iterator it;
    for (it = territoriosVecinos.begin(); it != territoriosVecinos.end(); ++it) {
        if (*it == vecino) {
            return true;
        }
    }
    return false;
}
