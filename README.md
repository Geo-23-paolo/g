# Gatochi

Gatochi es un juego de cuidado y simulación desarrollado en C++17 con SFML. El
jugador registra un gato, atiende sus necesidades y puede entrar a un
minijuego de laberinto para perseguir un ratón.

## Flujo general del juego

1. Al iniciar aparece la pantalla de presentación.
2. Presiona `Espacio` para abrir el registro.
3. Completa nombre, edad y género del gato.
4. Presiona `Enter` para comenzar.
5. Atiende al gato desde la pantalla principal.
6. Acércate a la puerta central y presiona `P` para entrar al laberinto.
7. En el laberinto, arrastra el ratón con el botón izquierdo y suéltalo dentro
   del mapa. El gato buscará automáticamente la ruta hacia él.
8. Cuando el gato toque al ratón, pierde una unidad de energía y vuelve al juego
   base.

## Controles

### Pantalla principal

| Tecla o acción | Función |
| --- | --- |
| `Espacio` | Abrir el registro al iniciar. |
| `Enter` | Confirmar el registro o seleccionar una opción. |
| `Backspace` | Borrar un carácter del registro. |
| `A` / `Flecha izquierda` | Mover al gato a la izquierda. |
| `D` / `Flecha derecha` | Mover al gato a la derecha. |
| `E` | Dormir cuando el gato está cerca de la cama. |
| `C` | Comer cuando hay comida disponible y el gato está cerca. |
| `M` | Usar el arenero cuando el gato está cerca. |
| `V` | Curar al gato cuando está enfermo. |
| `P` | Entrar al laberinto cuando el gato está cerca de la puerta. |
| `1` | Gastar una unidad de energía manualmente. |
| `3` | Activar el estado de Game Over para probarlo. |
| Clic izquierdo | Seleccionar la opción del veterinario cuando aparece. |

### Laberinto

| Tecla o acción | Función |
| --- | --- |
| `W` / `Flecha arriba` | Movimiento manual hacia arriba. |
| `A` / `Flecha izquierda` | Movimiento manual hacia la izquierda. |
| `S` / `Flecha abajo` | Movimiento manual hacia abajo. |
| `D` / `Flecha derecha` | Movimiento manual hacia la derecha. |
| Clic izquierdo sobre el ratón | Tomar el ratón. |
| Mover el cursor manteniendo el clic | Arrastrar el ratón. |
| Soltar el clic dentro del laberinto | Fijar el objetivo y activar A*. |
| `Escape` | Salir del laberinto sin gastar energía. |

Cuando existe un objetivo A*, el gato se mueve automáticamente y las teclas no
interrumpen esa ruta. Si el ratón se suelta fuera del laberinto, no se activa
ningún objetivo nuevo.

## Estados del gato

El juego mantiene una energía de `0` a `3` y muestra el estado actual del gato:

- `SANO`: tiene energía y sus necesidades están cubiertas.
- `CANSADO`: se quedó sin energía.
- `HAMBRIENTO`: la comida llegó a cero.
- `NECESITA IR AL BAÑO`: el estado del arenero requiere atención.
- `ENFERMO`: el gato permaneció demasiado tiempo sin energía.

La energía se puede perder manualmente con `1`, por el paso del tiempo mientras
se mueve o al completar el minijuego cuando alcanza al ratón. Dormir recupera
la energía y comer modifica la cantidad de comida disponible. El veterinario
puede curar al gato y la opción `Revivir` permite continuar después de Game
Over.

## Lógica del laberinto

La matriz editable se encuentra directamente en `src/Juego.cpp`, en la variable
`labyrinthMap`.

```cpp
'0' = muro, no transitable
'1' = camino, transitable
```

La cantidad de filas y columnas se obtiene automáticamente con:

```cpp
const int labyrinthRows = static_cast<int>(labyrinthMap.size());
const int labyrinthColumns = static_cast<int>(labyrinthMap.front().size());
```

El gato tiene una huella lógica de `2 x 2` cuadros:

```cpp
const int labyrinthCatLogicalSize = 2;
```

Antes de permitir una posición, la colisión revisa los cuatro cuadros que
ocupa el gato. Por eso una posición sólo es válida si las cuatro celdas
correspondientes contienen `1`. La escala, las texturas y el tamaño visual del
gato no dependen de esta huella lógica.

Para modificar el laberinto, cambia los caracteres de `labyrinthMap`:

- Cambia `1` por `0` para crear un muro.
- Cambia `0` por `1` para abrir un camino.
- Conserva la misma cantidad de caracteres en todas las filas.

## Algoritmo A*

El algoritmo está implementado en `findMousePath` dentro de `src/Juego.cpp`.

1. Convierte la posición del gato y del ratón en coordenadas de la matriz.
2. Coloca la posición inicial en una cola de prioridad.
3. Explora las cuatro direcciones: derecha, izquierda, abajo y arriba.
4. Descarta celdas con `0` y posiciones donde no cabe la huella `2 x 2`.
5. Usa un coste de `1` por cada paso.
6. Usa la distancia Manhattan como heurística.
7. Guarda el padre de cada celda para reconstruir la ruta más corta.
8. El gato sigue las celdas reconstruidas hasta alcanzar el ratón.

Si no existe una ruta válida, el gato permanece quieto y el jugador puede
soltar el ratón en otra posición.

## Captura del ratón

El ratón se carga desde `assets/Images/Raton.png` y aparece inicialmente en la
parte superior derecha, fuera del laberinto. El arrastre utiliza los eventos de
SFML `MouseButtonPressed`, `MouseMoved` y `MouseButtonReleased`.

Cuando las áreas globales del gato y del ratón se intersectan:

1. Se descuenta una unidad de energía, si todavía queda energía.
2. Se actualiza la interfaz de energía.
3. Se cancela el camino A*.
4. El gato vuelve a la pantalla principal.

## Estados de pantalla

El bucle principal controla las siguientes pantallas y transiciones:

- Presentación.
- Registro.
- Transición de entrada al juego.
- Juego base.
- Laberinto.
- Veterinario.
- Game Over.

La tecla `Escape` sólo sale del laberinto y restaura al gato en la pantalla
principal. El objetivo A* anterior se cancela al salir.

## Tecnologías

- C++17.
- SFML Graphics, Window, System y Audio.
- MinGW-w64.
- Make.

## Estructura del proyecto

| Carpeta | Contenido |
| --- | --- |
| `assets/Fonts/` | Fuentes de la interfaz. |
| `assets/Images/` | Fondos, sprites, matriz de referencia y ratón. |
| `assets/Music/` | Música y sonidos del juego. |
| `bin/` | Objetos y ejecutable generado. |
| `docs/` | Manuales y diagramas. |
| `include/` | Clases y declaraciones C++. |
| `src/` | Código fuente del juego. |

## Instalación y compilación

En una terminal de MSYS2 MinGW 64-bit instala las dependencias:

```sh
pacman -Syu
pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-sfml make pkgconf
```

Desde la raíz del proyecto compila con:

```sh
make build
```

El ejecutable se genera en `bin/gatochi.exe`. Para ejecutar usando la
configuración del Makefile:

```sh
make run
```

## Documentación adicional

- [Manual de usuario](docs/manual-usuario.md)
- [Guía de desarrollo](docs/Desarrollo.md)
- [Instalación detallada](docs/instalación.md)
- [Organización de assets](docs/assets.md)
- [Diagrama de estados](docs/DiagramaDeEstados.puml)
- [Diagrama del proyecto](docs/Gatochi.puml)

## Licencia

Gatochi se distribuye bajo la [licencia MIT](LICENSE).