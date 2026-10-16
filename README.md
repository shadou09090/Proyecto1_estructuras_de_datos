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
costo_conquista Hugo 5.6
conquista_mas_barata Hugo
obtener_unidades Hugo
```

La aplicación pide territorio y cantidad hasta ubicar todos los refuerzos.
`ayuda` muestra los comandos disponibles para la fase actual; `ayuda comando`
permite consultar cualquier sintaxis, incluso cuando el comando aún no está disponible.
Luego se ejecuta `atacar Hugo`; escribir `fin` permite terminar la fase.
`fortificar Hugo` permite mover entre dos territorios propios vecinos,
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

`archivo_inicio.txt` contiene a Hugo (Azul), Paco (Rojo) y Luis (Verde).
El `0` de cada jugador indica que comienza sin cartas. A continuación, el `8`
indica la cantidad de cartas del mazo, con identificadores del 101 al 108;
cada fila contiene identificador, código de territorio y figura. Después siguen
las 42 filas de territorios con código, color y unidades. Este formato se carga
directamente con `inicializar archivo_inicio.txt`; el primer turno es de Hugo.

- **Carga inicial:** 3 a 6 jugadores, nombres únicos de hasta 8 bytes, colores
  verde, azul, rojo, amarillo, negro o gris; 42 códigos únicos y unidades
  positivas. Los totales iniciales son 35/30/25/20 según el número de jugadores.
- **Guardado:** sigue el ejemplo del profesor: cantidad de jugadores; una fila
  por jugador con nombre, color, cantidad de cartas y sus ternas de identificador,
  territorio y figura; cantidad de cartas del mazo; una terna por carta del mazo;
  finalmente las 42 filas de territorios. Se conservan mayúsculas e identificadores.
  `guardar_juego` es un alias de `guardar`.
- **Auxiliar:** turno, fases y contador de canjes se conservan en `.estado`.
  Sin ese auxiliar, el formato del profesor conserva manos y mazo, pero inicia
  con el primer jugador vivo, fases nuevas y contador de canjes en cero.
  Los archivos antiguos sin cartas también se pueden cargar; se genera un mazo.
- **Comandos del profesor:** las líneas que comienzan por `#` se ignoran.
  La secuencia completa de `juego_comandosv1.txt` todavía no es compatible:
  el juego conserva su interfaz interactiva de refuerzos, ataque y fortificación.
  El formato de los archivos de estado sí está implementado y verificado.
- **Huffman:** formato `n(2) [carácter(1), frecuencia(8)] w(8) bits`, enteros
  little-endian, bits más significativos primero y relleno cero. Empates del
  árbol se resuelven por menor código de símbolo del subárbol. El texto recuperado
  coincide exactamente con el archivo principal producido por `guardar`;
  no incluye el auxiliar de estado dentro del binario.
  Se rechazan truncamiento, frecuencias inconsistentes, duplicados y bytes extra.
  La carga admite archivos/descompresión de hasta 4 MiB.
- **Cartas:** cuando el TXT incluye cartas, se conservan exactamente las manos,
  el mazo, sus identificadores y figuras (ocho cartas en el archivo de inicio).
  Para archivos antiguos sin cartas se generan 42 cartas de territorio con
  dibujos cíclicos (14 de cada tipo) y dos comodines.
  Se entrega una carta por fase con conquista, con transferencia al eliminar un
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
