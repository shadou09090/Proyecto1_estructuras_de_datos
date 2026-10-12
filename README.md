# Risk - Estructuras de datos

Se conservan `Juego`, `Jugador` y `Territorio`, la consola y los comandos originales.
Las funciones nuevas se separan en `Huffman.cpp` (árbol y codificación) y
`JuegoEstado.cpp` (persistencia y cartas). El código no utiliza `auto`.

## Compilar y probar

Con GNU g++ y C++11:

```sh
g++ -std=c++11 -Wall -Wextra -Wpedantic main.cpp Juego.cpp JuegoEstado.cpp Huffman.cpp Jugador.cpp Territorio.cpp -o risk
./risk
```

En Windows, usa `-o risk.exe` y ejecuta `.\risk.exe` desde una carpeta con
`archivo_inicio.txt`. `Archivos.h` reemplaza las operaciones de archivos que
requerían C++17; no necesita bibliotecas adicionales.

Alternativa con CMake/CLion:

```sh
cmake -S . -B build
cmake --build build
```

## Uso

```text
ayuda
ayuda guardar_comprimido
inicializar archivo_inicio.txt
estado_juego
costo_conquista nikolas 5.6
conquista_mas_barata nikolas
obtener_unidades nikolas
```

La aplicación pide territorio y cantidad hasta ubicar todos los refuerzos.
`ayuda` muestra los comandos disponibles para la fase actual; `ayuda comando`
permite consultar cualquier sintaxis, incluso cuando el comando aún no está disponible.
Luego se ejecuta `atacar nikolas`; escribir `fin` permite terminar la fase.
`fortificar nikolas` permite mover entre dos territorios propios vecinos,
o escribir `fin` para pasar al siguiente jugador sin mover.

```text
guardar "Mi partida.txt"
guardar_comprimido "Mi partida.bin"
salir
```

En una nueva ejecución, `inicializar "Mi partida.bin"` recupera el estado.
Se conservan mayúsculas y espacios del nombre de archivo. Si el nombre de
guardado comprimido no termina en `.bin`, se agrega esa extensión.

## Decisiones de implementación

- **Carga inicial:** 3 a 6 jugadores, nombres únicos de hasta 8 bytes, colores
  verde, azul, rojo, amarillo, negro o gris; 42 códigos únicos y unidades
  positivas. Los totales iniciales son 35/30/25/20 según el número de jugadores.
- **Guardado:** el archivo principal contiene únicamente el encabezado, los
  jugadores y las 42 filas del enunciado. Turno, fases, canjes y cartas se guardan
  en un archivo auxiliar con el mismo nombre más `.estado`. Conserva ambos
  archivos juntos para recuperar la partida completa. Primero se escriben
  temporales; si falla el reemplazo, se intenta restaurar la versión anterior.
  El auxiliar se valida contra el tablero. Se aceptan saltos de línea LF y CRLF.
- **Sin auxiliar:** se valida como configuración inicial. Si los totales de tropas
  no son iniciales, pero el tablero es válido, el programa pregunta si se trata
  de una partida guardada. Al confirmar se recupera el tablero y se detectan
  jugadores eliminados y ganador, pero las fases, las manos y los canjes se
  reinician, con turno del primer jugador vivo. Un auxiliar presente y corrupto
  se rechaza para no perder silenciosamente datos de la partida.
- **Huffman:** formato `n(2) [carácter(1), frecuencia(8)] w(8) bits`, enteros
  little-endian, bits más significativos primero y relleno cero. Empates del
  árbol se resuelven por menor código de símbolo del subárbol. El texto recuperado
  coincide exactamente con el archivo principal producido por `guardar`;
  no incluye el auxiliar de estado dentro del binario.
  Se rechazan truncamiento, frecuencias inconsistentes, duplicados y bytes extra.
  La carga admite archivos/descompresión de hasta 4 MiB.
- **Cartas:** 42 cartas de territorio con dibujos cíclicos (14 de cada tipo),
  dos comodines, una carta por fase con conquista, transferencia al eliminar un
  rival, devolución al mazo tras canje. No se incluyen misiones secretas:
  se juega la variante de conquista de los 42 territorios.
- **Rutas:** grafo del tablero clásico; Dijkstra multiorigen, todos los
  territorios propios a costo cero y cada territorio enemigo con peso igual
  a sus tropas. "Más cerca" se interpreta como menor costo de tropas. El costo
  es una estimación estratégica, no una simulación probabilística de pérdidas.
  Empates conservan la primera ruta hallada con orden lexicográfico de códigos.
  La carga completa la lista de vecinos de cada `Territorio` a partir del mismo
  mapa utilizado en las consultas, ataques y fortificaciones.
- **Entrada incompleta:** al llegar EOF durante refuerzos o ataque, se revierte
  esa fase completa. Una entrada inválida ordinaria permite volver a intentarlo.

## Documentación y procedencia

`output/pdf/diseno_y_pruebas.pdf` es un borrador de una versión anterior, pendiente
de actualizar al guardado con auxiliar y a la ayuda por fases. Para ejecutar esta
versión, sigue este README y el código. Los cambios realizados con apoyo de Codex están identificados mediante
comentarios. Prompts: «termina lo que falte», «continúa» y «no puedes usar auto y
trata de no cambiar demasiado del proyecto original».

El enunciado exige la plantilla de TAD vista en clase y comentarios literales de
evaluaciones anteriores. Esos materiales no fueron aportados; el documento no
inventa actas ni comentarios del profesor. Tampoco acredita el límite de contenido
asistido por IA que establece el enunciado. Revisar esos puntos antes de presentar
la entrega. No se modificaron los PDF originales ni se publicó el proyecto.

## Verificación del programa

La adaptación a C++11 se verificó con `-Wall -Wextra -Wpedantic -Werror` y con
pruebas de guardado, sobrescritura, recuperación de fases, nombres con espacios,
errores de escritura, rutas de conquista y codificación/decodificación Huffman.
El ejecutable también se compila mediante CMake con `CMAKE_CXX_STANDARD 11`.
El archivo `tests.cpp` ya no está en esta carpeta; CMake solo crea ese objetivo
opcional si el archivo está presente.
