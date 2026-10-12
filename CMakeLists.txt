cmake_minimum_required(VERSION 3.20)
project(proyecto_estructuras_pais)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MINGW)
    add_link_options(-static-libgcc -static-libstdc++)
endif()

add_executable(proyecto_estructuras_pais main.cpp
        Juego.cpp
        JuegoEstado.cpp
        Huffman.cpp
        Archivos.h
        Juego.h
        Territorio.cpp
        Territorio.h
        Jugador.cpp
        Jugador.h)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/archivo_inicio.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

include(CTest)
if(BUILD_TESTING AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests.cpp")
    add_executable(risk_tests tests.cpp Juego.cpp JuegoEstado.cpp Huffman.cpp Jugador.cpp Territorio.cpp)
    target_compile_definitions(risk_tests PRIVATE RISK_TESTS)
    add_test(NAME risk_tests COMMAND risk_tests)
    set_tests_properties(risk_tests PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} TIMEOUT 30)
endif()
