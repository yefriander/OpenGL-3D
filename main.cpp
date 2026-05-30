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

using namespace std;

/// Colores
float amarillo[]={1,1,0}, blanco[]={1,1,1}, azul[]={0,0,1}, rojo[]={1,0,0}, anaranjado[]={1,0.8,0}, verde[]={0,1,0};

/// Variables globales de la cámara
float radio  = 12.0;                // distancia de la cámara al origen
float yaw    = 45 * M_PI / 180.0f;   // ángulo horizontal inicial
float pitch  = 30 * M_PI / 180.0f;   // ángulo vertical inicial

/// Variables de estado del mouse
bool mouse_presionado = false;
float mouse_x_ant = 0;
float mouse_y_ant = 0;
float sensibilidad = 0.005;  // cuánto gira por pixel arrastrado

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

/// PASO 6a: Callback de botón del mouse
void mouse_boton(int boton, int estado, int x, int y){
    if (boton == GLUT_LEFT_BUTTON){
        mouse_presionado = (estado == GLUT_DOWN);
        mouse_x_ant = x;
        mouse_y_ant = y;
    }
}

/// PASO 6b: Callback de movimiento del mouse (con botón presionado)
void mouse_movimiento(int x, int y)
{
    if (!mouse_presionado)
        return;

    int delta_x = x - mouse_x_ant; // movimiento horizontal del mouse
    int delta_y = y - mouse_y_ant; // movimiento vertical del mouse

    yaw   += delta_x * sensibilidad; // girar cámara izq/der
    pitch -= delta_y * sensibilidad; // girar cámara arriba/abajo

    // Limitar pitch para que la cámara no gire más de ±89°
    float limite = 89.0f * M_PI / 180.0f;

    if (pitch > limite)  pitch = limite;
    if (pitch < -limite) pitch = -limite;

    mouse_x_ant = x;
    mouse_y_ant = y;

    glutPostRedisplay(); // solicitar que se redibuje el frame
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

/**
OBJETO 1 — Solo componente AMBIENTAL
El objeto se ve completamente plano y opaco.
No importa desde dónde venga la luz ni el ángulo de visión:
todos los píxeles tienen el mismo color. Sin volumen.
**/
void dibujar_ambiental(){
    GLfloat mat_ambient[]  = {0.6f, 0.0f, 0.6f, 1.0f}; // color ambiental: morado
    GLfloat mat_diffuse[]  = {0.0f, 0.0f, 0.0f, 1.0f}; // sin difusa
    GLfloat mat_specular[] = {0.0f, 0.0f, 0.0f, 1.0f}; // sin especular

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialf (GL_FRONT, GL_SHININESS, 0.0);

    glPushMatrix();
    glTranslatef(-3.5, 0, 0);        // posicionar a la izquierda
    glutSolidSphere(1.0, 32, 32);
    glPopMatrix();
}

/**
OBJETO 2 — AMBIENTAL + DIFUSA
Ahora el objeto tiene volumen: las caras orientadas hacia
la luz se iluminan más, las opuestas quedan en sombra.
La componente difusa es la más importante para dar forma.
**/
void dibujar_difusa(){
    GLfloat mat_ambient[]  = {0.1f, 0.3f, 0.1f, 1.0f}; // ambiental verde oscuro
    GLfloat mat_diffuse[]  = {0.2f, 0.9f, 0.2f, 1.0f}; // difusa verde brillante
    GLfloat mat_specular[] = {0.0f, 0.0f, 0.0f, 1.0f}; // sin especular

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);  // ambiental verde oscuro
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);  // difusa verde brillante
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);  // sin especular
    glMaterialf (GL_FRONT, GL_SHININESS, 0.0);

    glPushMatrix();
    glTranslatef(0, -0.5, 0);        // centrado, bajado un poco por la tetera
    glutSolidTeapot(1.0);
    glPopMatrix();
}

/**
OBJETO 3 — AMBIENTAL + DIFUSA + ESPECULAR
El reflejo especular agrega un punto brillante (highlight)
que se mueve según el ángulo entre la luz y la cámara.
GL_SHININESS controla el tamaño del reflejo:
valor bajo  (1-10)  -> reflejo grande y difuso (material mate)
valor alto (64-128) -> reflejo pequeño y concentrado (material brillante)
**/
void dibujar_especular(){
    GLfloat mat_ambient[]  = {0.1f, 0.1f, 0.3f, 1.0f}; // ambiental azul oscuro
    GLfloat mat_diffuse[]  = {0.2f, 0.2f, 0.8f, 1.0f}; // difusa azul
    GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f}; //reflejo blanco puro

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);   // ambiental azul oscuro
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);   // difusa azul
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);  // reflejo blanco puro
    glMaterialf (GL_FRONT, GL_SHININESS, 96.0);                  // muy concentrado (tipo metal)

    glPushMatrix();
    glTranslatef(3.5, 0, 0);         // posicionar a la derecha
    glutSolidSphere(1.0, 32, 32);
    glPopMatrix();
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
