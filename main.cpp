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
    glEnable(GL_LIGHTING);   // sin esto glColor3f sigue funcionando pero no hay luces
    glEnable(GL_LIGHT0);     // activar la fuente de luz 0 (OpenGL soporta hasta 8: LIGHT0..LIGHT7)
    glEnable(GL_NORMALIZE);  // normalizar automáticamente los vectores normales (importante con glScale)

    /// --- Propiedades de la fuente de luz LIGHT0 ---
    // Componente AMBIENTAL de la luz: intensidad de la luz de fondo
    GLfloat luz_ambiente[] = {0.3f, 0.3f, 0.3f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT,  luz_ambiente);
    // Componente DIFUSA de la luz: intensidad de la luz direccional
    GLfloat luz_difuso[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  luz_difuso);
    // Componente ESPECULAR de la luz: intensidad del reflejo
    GLfloat luz_especular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_SPECULAR, luz_especular);
    // Nota: la POSICIÓN de la luz se fija en display() después de gluLookAt
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
    GLfloat posicion_luz[] = {4.0f, 6.0f, 4.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, posicion_luz);

    dibujar_ejes();

    /// Los 3 objetos demuestran cada componente de iluminación
    dibujar_ambiental();    // izquierda:  solo ambiental  (plano, sin volumen)
    dibujar_difusa();       // centro:     ambiental+difusa (con volumen, sin brillo)
    dibujar_especular();    // derecha:    los 3 componentes (volumen + reflejo brillante)

    // dibujar_cubo();  /// PASO 4: objeto dibujado en 3D

    // saludar();

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
    glutMouseFunc(mouse_boton);        // click / soltar
    glutMotionFunc(mouse_movimiento);  // arrastrar con botón presionado

    inicio();
    glutMainLoop();

    return EXIT_SUCCESS;
}
