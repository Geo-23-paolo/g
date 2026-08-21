# Instalación

## Dependencias

En MSYS2, abre una terminal **MinGW 64-bit** e instala MinGW-w64, SFML y las
herramientas de compilación:

```sh
pacman -Syu
pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-sfml make pkgconf
```

## Compilar

Desde la raíz del proyecto ejecuta:

```sh
make
```

El resultado será `bin/gatochi.exe`.