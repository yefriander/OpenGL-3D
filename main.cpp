#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include <stdlib.h>
#include <math.h> // Necesario para cos() y sin()
#include "funciones.h"
#include "mouse.h"
#include "objetos.h"
#include "escena.h"

using namespace std;

/// Colores
float amarillo[]={1,1,0}, blanco[]={1,1,1}, azul[]={0,0,1}, rojo[]={1,0,0}, anaranjado[]={1,0.8,0}, verde[]={0,1,0};

void inicio ()
{
    /// PASO 1: activar depth test
    glEnable(GL_DEPTH_TEST);

    /// PASO 2: cambiar a proyección 3D con perspectiva
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 600/600, 0.1, 100);

    /// Activar el sistema de iluminación
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);

    /// LIGHT0 — luz ambiente general (blanca, desde arriba)
    GLfloat luz0_ambiente[]  = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat luz0_difuso[]    = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat luz0_especular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT,  luz0_ambiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  luz0_difuso);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luz0_especular);

    /// LIGHT1 — lámpara colgante (amarilla cálida, posicional)
    GLfloat luz1_ambiente[]  = {0.05f, 0.04f, 0.0f,  1.0f};
    GLfloat luz1_difuso[]    = {0.9f,  0.75f, 0.35f, 1.0f};
    GLfloat luz1_especular[] = {0.8f,  0.65f, 0.3f,  1.0f};
    glLightfv(GL_LIGHT1, GL_AMBIENT,  luz1_ambiente);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  luz1_difuso);
    glLightfv(GL_LIGHT1, GL_SPECULAR, luz1_especular);
    // Nota: posición de ambas luces se fija en display() después de gluLookAt
}

/// PASO 4: Dibujar en 3D usando glVertex3f(x, y, z)
void dibujar_cubo(){
    glBegin(GL_QUADS);

    /// Cara frontal  (z = +1)
    glColor3f(1, 0, 0); /// Rojo
    glVertex3f(-1, -1,  1);
    glVertex3f( 1, -1,  1);
    glVertex3f( 1,  1,  1);
    glVertex3f(-1,  1,  1);
    

    /// Cara trasera  (z = -1)
    glColor3f(0, 1, 0); /// Verde
    glVertex3f(-1, -1, -1);
    glVertex3f(-1,  1, -1);
    glVertex3f( 1,  1, -1);
    glVertex3f( 1, -1, -1);

    /// Cara superior (y = +1)
    glColor3f(0, 0, 1); /// Azul
    glVertex3f(-1,  1, -1);
    glVertex3f(-1,  1,  1);
    glVertex3f( 1,  1,  1);
    glVertex3f( 1,  1, -1);

    /// Cara inferior (y = -1)
    glColor3f(1, 1, 0); /// Amarillo
    glVertex3f(-1, -1, -1);
    glVertex3f( 1, -1, -1);
    glVertex3f( 1, -1,  1);
    glVertex3f(-1, -1,  1);

    /// Cara derecha  (x = +1)
    glColor3f(1, 0, 1); /// Magenta (rojizo purpura)
    glVertex3f( 1, -1, -1);
    glVertex3f( 1,  1, -1);
    glVertex3f( 1,  1,  1);
    glVertex3f( 1, -1,  1);

    /// Cara izquierda (x = -1)
    glColor3f(0, 1, 1); /// Cyan parecido al celeste
    glVertex3f(-1, -1, -1);
    glVertex3f(-1, -1,  1);
    glVertex3f(-1,  1,  1);
    glVertex3f(-1,  1, -1);

    glEnd();
}

void dibujar_ejes(){
    /// Desactivar iluminación para los ejes: así glColor3f funciona
    /// (con GL_LIGHTING activo, glColor3f es ignorado)
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    /// Eje X -> rojo
    glColor3f(1, 0, 0);
    glVertex3f(-5, 0, 0);
    glVertex3f( 5, 0, 0);
    /// Eje Y -> verde
    glColor3f(0, 1, 0);
    glVertex3f(0, -5, 0);
    glVertex3f(0,  5, 0);
    /// Eje Z -> azul
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, -5);
    glVertex3f(0, 0,  5);
    glEnd();
    glEnable(GL_LIGHTING);  // volver a activar para los objetos
}

void display(void)
{
    glClearColor(0.2, 0.2, 0.2, 1);
    /// PASO 5: Limpiar el Depth Buffer en cada frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /// PASO 6c: recalcular posición de la cámara desde los ángulos
    float cam_x = radio * cos(pitch) * sin(yaw);
    float cam_y = radio * sin(pitch);
    float cam_z = radio * cos(pitch) * cos(yaw);

    /// PASO 3: posicionar la cámara en el espacio 3D
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cam_x, cam_y, cam_z,    // posición de la cámara (se actualiza cada frame)
              0, 0, 0,                // siempre mira al origen donde está el cubo
              0, 1, 0);               // eje Y como "arriba"

    /// Posición de la luz en coordenadas del mundo (se fija aquí, después de gluLookAt)
    /// El cuarto valor (1.0) indica luz puntual; con 0.0 sería luz direccional (infinita)
    GLfloat posicion_luz0[] = {4.0f, 6.0f, 4.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, posicion_luz0);

    GLfloat posicion_luz1[] = {0.0f, 3.0f, 0.0f, 1.0f};  // dentro de la lámpara
    glLightfv(GL_LIGHT1, GL_POSITION, posicion_luz1);

    dibujar_ejes();
    dibujar_escena();

    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(600,600);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);

    glutCreateWindow("OpenGL 3D");
    glutDisplayFunc(display);

    /// PASO 6d: registrar los callbacks del mouse en GLUT
    glutMouseFunc(mouse_boton);
    glutMotionFunc(mouse_movimiento);

    inicio();
    glutMainLoop();

    return EXIT_SUCCESS;
}
