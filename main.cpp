#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif

#include "Juego.h"
#include "Jugador.h"
using  namespace  std;

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    Juego J;
    J.Empezar_a_jugar();

    return 0;
}