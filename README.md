# OpenGL 3D

Proyecto de OpenGL con iluminación 3D usando GLUT/FreeGLUT. Muestra tres esferas con distintas componentes de iluminación (ambiental, difusa, especular) y rotación de cámara con el mouse.

## Requisitos

- [Code::Blocks con MinGW](https://www.codeblocks.org/downloads/) — incluye el compilador `g++`
- [FreeGLUT para MinGW](https://www.transmissionzero.co.uk/software/freeglut-devel/) — descargar la versión MinGW y colocar las carpetas `bin`, `include` y `lib` dentro de una carpeta `glut/` en la raíz del proyecto

La estructura esperada es:
```
glut/
  include/GL/
  lib/
```

## Compilar y ejecutar

Abrir el proyecto en VS Code y presionar **F5**.

O desde terminal:
```
g++ main.cpp -Iglut/include -Lglut/lib -DFREEGLUT_STATIC -lfreeglut_static -lopengl32 -lglu32 -lwinmm -lgdi32 -o app.exe
./app.exe
```

## Controles

- **Click izquierdo + arrastrar** — rotar la cámara