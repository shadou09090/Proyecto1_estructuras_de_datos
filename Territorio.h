//
// Created by gniko on 29/08/2026.
//
using  namespace  std;

#ifndef PROYECTO_ESTRUCTURAS_PAIS_TERRITORIO_H
#define PROYECTO_ESTRUCTURAS_PAIS_TERRITORIO_H
#include <string>
#include <list>

class Territorio {
private:
    std::string nombre;
    int numero;
    string colorOcupante;
    int numUnidades;
    list<string> territoriosVecinos;

public:
    Territorio(string nombre);

    string getNombre();
    int getNumero();
    void setNumero(int numero);
    void setNombre(string nombre);

    string getColorOcupante();
    void setColorOcupante(string colorOcupante);

    int getNumUnidades();
    void setNumUnidades(int numUnidades);
    void agregarVecino(string vecino);
    bool esVecino(string vecino);
};


#endif //PROYECTO_ESTRUCTURAS_PAIS_TERRITORIO_H