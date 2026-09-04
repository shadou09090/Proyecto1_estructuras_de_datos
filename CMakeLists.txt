cmake_minimum_required(VERSION 3.20)
project(proyecto_estructuras_pais)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

add_executable(proyecto_estructuras_pais main.cpp
        Juego.cpp
        Juego.h
        Territorio.cpp
        Territorio.h
        Jugador.cpp
        Jugador.h)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/archivo_inicio.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
