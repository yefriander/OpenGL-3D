#ifndef TEXTURAS_H
#define TEXTURAS_H

#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Carga un BMP de 24 bits y devuelve el ID de textura OpenGL (0 = error)
GLuint cargar_textura(const char* archivo);

// Cubo unitario centrado en el origen con textura en las 6 caras
void dibujar_cubo_texturizado(GLuint tex);

// Quad vertical (pared) centrado en X, con base en Y=0
void dibujar_pared_texturizada(GLuint tex, float ancho, float alto, float repeticiones = 1.0f);

// Quad horizontal (piso) centrado en origen, sobre Y=0
void dibujar_piso_texturizado(GLuint tex, float ancho, float profundo, float repeticiones = 4.0f);

#endif