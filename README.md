# 🐱 Gatochi

> **Cuida, juega y acompaña a tu gato.** 🐾

![Estado del proyecto](https://img.shields.io/badge/estado-en%20desarrollo-orange)
![Lenguaje](https://img.shields.io/badge/C%2B%2B-17-blue)
![Licencia](https://img.shields.io/badge/licencia-MIT-green)

**Gatochi** es un juego de cuidado y simulación en el que asumirás el papel de
una persona responsable de su propio gato. Tu misión será conocer sus
necesidades, atenderlo a tiempo y construir una rutina que lo mantenga feliz,
saludable y activo. 🌟

---

## 🎮 ¿De qué trata?

En Gatochi, cada decisión cuenta. El gato tendrá diferentes necesidades y el
jugador deberá observar su estado para decidir qué hacer en cada momento.

### 🐾 Actividades principales

| Acción | Objetivo |
| --- | --- |
| 🍽️ Alimentar | Satisfacer el hambre del gato. |
| 🎾 Jugar | Mantenerlo activo y de buen ánimo. |
| 😴 Dormir | Permitirle recuperar energía. |
| 🧹 Limpiar | Mantener el arenero y su entorno en buenas condiciones. |
| 🩺 Visitar al veterinario | Cuidar su salud cuando necesite atención. |

La idea es ofrecer una experiencia sencilla, cercana y entretenida, con espacio
para añadir nuevas actividades, estados, personajes y recursos multimedia. ✨

## 🚧 Estado del proyecto

Gatochi se encuentra actualmente **en desarrollo**. El repositorio ya cuenta
con:

- La base del modelo de clases del juego.
- Configuración de compilación con MinGW-w64 y SFML.
- Estructura para imágenes, fuentes, música y sonidos.
- Documentación inicial y diagrama UML.

## 🛠️ Tecnologías

- **C++17**: lenguaje principal.
- **SFML**: ventanas, gráficos, audio e interacción.
- **MinGW-w64**: generación del ejecutable para Windows.
- **Make**: automatización de la compilación.

## 📁 Estructura del proyecto

| Carpeta | Descripción |
| --- | --- |
| `.VSCode/` | Configuración opcional del editor. |
| `assets/Fonts/` | Fuentes utilizadas por la interfaz. |
| `assets/Images/` | Imágenes, sprites y texturas. |
| `assets/Music/` | Música y efectos de sonido fuente. |
| `bin/Soundtrack/` | Archivos de audio preparados para ejecución. |
| `docs/` | Manuales, diagramas y documentación técnica. |
| `include/` | Archivos de cabecera `.hpp`. |
| `src/` | Implementaciones `.cpp`. |

## ⚙️ Instalación y compilación

### 1. Instalar dependencias

En MSYS2, abre una terminal **MinGW 64-bit** y ejecuta:

```sh
pacman -Syu
pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-sfml make pkgconf
```

### 2. Compilar el proyecto

Desde la raíz del repositorio:

```sh
make
```

El ejecutable se generará en `bin/gatochi.exe`.

### 3. Ejecutar

Si tienes Wine disponible, puedes iniciar el juego con:

```sh
make run
```

Para una instalación personalizada de SFML, indica sus rutas manualmente:

```sh
make SFML_CFLAGS="-IC:/ruta/a/SFML/include" \
     SFML_LIBS="-LC:/ruta/a/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system"
```

## 📚 Documentación

- 📖 [Manual de usuario](docs/manual-usuario.md)
- 💻 [Guía de desarrollo](docs/Desarrollo.md)
- 📦 [Instalación detallada](docs/instalación.md)
- 🎨 [Organización de assets](docs/assets.md)
- 🧩 [Diagrama UML](docs/Diagrama%20de%20Proyercto.puml)

## 💬 Contacto y colaboración

¿Tienes una idea, encontraste un error o quieres conocer más sobre el
proyecto? Puedes comunicarte mediante:

- 🐛 [Issues del repositorio](https://github.com/Geo-23-paolo/Gatochi/issues)
  para reportar problemas o proponer mejoras.
- 🔀 Pull requests para contribuir directamente al código.
- 📝 Comentarios y sugerencias en las discusiones del proyecto.

Al reportar un problema, incluye los pasos para reproducirlo, tu sistema
operativo y la versión de SFML utilizada. ¡Toda ayuda es bienvenida! 🙌

## 📄 Licencia

Gatochi se distribuye bajo la [licencia MIT](LICENSE).

---

Hecho con 💙 para quienes disfrutan cuidar, jugar y compartir tiempo con sus
compañeros felinos.
