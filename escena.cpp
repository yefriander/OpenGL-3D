#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "escena.h"

static void set_material(GLfloat ar, GLfloat ag, GLfloat ab,
                         GLfloat dr, GLfloat dg, GLfloat db,
                         GLfloat shininess)
{
    GLfloat ambient[]  = {ar, ag, ab, 1.0f};
    GLfloat diffuse[]  = {dr, dg, db, 1.0f};
    GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  specular);
    glMaterialf (GL_FRONT, GL_SHININESS, shininess);
}

static void dibujar_piso()
{
    set_material(0.15f, 0.08f, 0.02f, 0.30f, 0.18f, 0.07f, 8.0f);
    glPushMatrix();
        glTranslatef(0, -1.65f, 0);
        glScalef(14.0f, 0.1f, 14.0f);
        glutSolidCube(1.0);
    glPopMatrix();
}

static void dibujar_pared_fondo()
{
    set_material(0.3f, 0.28f, 0.25f, 0.65f, 0.62f, 0.55f, 5.0f);
    glPushMatrix();
        glTranslatef(0, 2.5f, -4.0f);
        glScalef(14.0f, 10.0f, 0.1f);
        glutSolidCube(1.0);
    glPopMatrix();
}

static void dibujar_cuadro_pared()
{
    // Marco de madera oscura
    set_material(0.2f, 0.1f, 0.02f, 0.4f, 0.2f, 0.05f, 40.0f);
    glPushMatrix();
        glTranslatef(-1.5f, 2.8f, -3.93f);
        glScalef(2.4f, 1.8f, 0.06f);
        glutSolidCube(1.0);
    glPopMatrix();

    // Lienzo azul/verde
    set_material(0.05f, 0.15f, 0.2f, 0.1f, 0.4f, 0.6f, 15.0f);
    glPushMatrix();
        glTranslatef(-1.5f, 2.8f, -3.90f);
        glScalef(2.0f, 1.4f, 0.02f);
        glutSolidCube(1.0);
    glPopMatrix();
}

static void dibujar_lampara()
{
    // Pantalla (cono: base ancha abajo, punta arriba)
    set_material(0.35f, 0.3f, 0.05f, 0.85f, 0.75f, 0.1f, 50.0f);
    glPushMatrix();
        glTranslatef(0, 3.0f, 0);
        glRotatef(-90.0f, 1, 0, 0);   // Z→Y: base en Y=3.0, punta en Y=3.5
        glutSolidCone(0.55f, 0.5f, 32, 8);
    glPopMatrix();

    // Cable delgado: de la punta (Y=3.5) al techo (Y=5.0)
    set_material(0.05f, 0.05f, 0.05f, 0.15f, 0.15f, 0.15f, 10.0f);
    glPushMatrix();
        glTranslatef(0, 3.5f, 0);
        glRotatef(-90.0f, 1, 0, 0);   // cilindro sube por el eje Y
        GLUquadric* q = gluNewQuadric();
        gluCylinder(q, 0.02f, 0.02f, 1.5f, 8, 2);
        gluDeleteQuadric(q);
    glPopMatrix();
}

static void dibujar_mesa()
{
    set_material(0.25f, 0.12f, 0.04f, 0.55f, 0.27f, 0.07f, 20.0f);

    // Tablero
    glPushMatrix();
        glScalef(5.0f, 0.15f, 3.0f);
        glutSolidCube(1.0);
    glPopMatrix();

    // 4 patas
    float px[] = {-2.2f,  2.2f, -2.2f,  2.2f};
    float pz[] = {-1.2f, -1.2f,  1.2f,  1.2f};
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
            glTranslatef(px[i], -0.825f, pz[i]);
            glScalef(0.15f, 1.5f, 0.15f);
            glutSolidCube(1.0);
        glPopMatrix();
    }
}

static void dibujar_mantel()
{
    set_material(0.4f, 0.08f, 0.08f, 0.8f, 0.15f, 0.15f, 10.0f);
    glPushMatrix();
        glTranslatef(0, 0.09f, 0);
        glScalef(5.2f, 0.03f, 3.2f);
        glutSolidCube(1.0);
    glPopMatrix();
}

static void dibujar_libros()
{
    // 3 libros apilados en el extremo derecho trasero de la mesa
    float col[3][6] = {
        {0.3f, 0.05f, 0.05f,  0.7f, 0.1f, 0.1f},   // rojo
        {0.05f, 0.1f, 0.3f,   0.1f, 0.2f, 0.7f},   // azul
        {0.05f, 0.25f, 0.05f, 0.1f, 0.6f, 0.1f},   // verde
    };
    float alturas[]   = {0.12f, 0.20f, 0.28f};
    float rotaciones[] = {0.0f,  4.0f, -3.0f};

    for (int i = 0; i < 3; i++) {
        set_material(col[i][0], col[i][1], col[i][2],
                     col[i][3], col[i][4], col[i][5], 30.0f);
        glPushMatrix();
            glTranslatef(1.8f, alturas[i], -0.8f);
            glRotatef(rotaciones[i], 0, 1, 0);
            glScalef(0.9f, 0.07f, 0.65f);
            glutSolidCube(1.0);
        glPopMatrix();
    }
}

static void dibujar_silla()
{
    const float SX = 0.0f;
    const float SZ = 2.5f;   // frente a la mesa
    set_material(0.2f, 0.1f, 0.03f, 0.45f, 0.22f, 0.07f, 25.0f);

    // Asiento
    glPushMatrix();
        glTranslatef(SX, -0.4f, SZ);
        glScalef(1.2f, 0.08f, 1.2f);
        glutSolidCube(1.0);
    glPopMatrix();

    // Respaldo
    glPushMatrix();
        glTranslatef(SX, 0.2f, SZ + 0.55f);
        glScalef(1.2f, 1.15f, 0.07f);
        glutSolidCube(1.0);
    glPopMatrix();

    // 4 patas
    float lpx[] = {-0.5f,  0.5f, -0.5f,  0.5f};
    float lpz[] = {-0.5f, -0.5f,  0.5f,  0.5f};
    for (int i = 0; i < 4; i++) {
        float top = -0.44f, bot = -1.60f;
        glPushMatrix();
            glTranslatef(SX + lpx[i], (top + bot) * 0.5f, SZ + lpz[i]);
            glScalef(0.08f, top - bot, 0.08f);
            glutSolidCube(1.0);
        glPopMatrix();
    }
}

void dibujar_escena()
{
    dibujar_piso();
    dibujar_pared_fondo();
    dibujar_cuadro_pared();
    dibujar_lampara();
    dibujar_mesa();
    dibujar_mantel();
    dibujar_silla();

    const float Y = 0.08f;

    // Tetera — dorado
    set_material(0.25f, 0.20f, 0.02f, 0.8f, 0.65f, 0.1f, 80.0f);
    glPushMatrix();
        glTranslatef(-1.5f, Y + 0.3f, 0.4f);
        glutSolidTeapot(0.4);
    glPopMatrix();

    // Esfera — azul metálico
    set_material(0.05f, 0.05f, 0.25f, 0.1f, 0.2f, 0.8f, 96.0f);
    glPushMatrix();
        glTranslatef(0.0f, Y + 0.38f, 0.3f);
        glutSolidSphere(0.38, 32, 32);
    glPopMatrix();

    // Cono — rojo
    set_material(0.25f, 0.02f, 0.02f, 0.8f, 0.1f, 0.1f, 64.0f);
    glPushMatrix();
        glTranslatef(1.5f, Y, 0.4f);
        glRotatef(-90.0f, 1, 0, 0);
        glutSolidCone(0.35, 0.8, 32, 16);
    glPopMatrix();

    // Toroide — verde metálico
    set_material(0.02f, 0.2f, 0.05f, 0.1f, 0.7f, 0.15f, 80.0f);
    glPushMatrix();
        glTranslatef(-1.5f, Y + 0.12f, -0.6f);
        glRotatef(90.0f, 1, 0, 0);
        glutSolidTorus(0.12, 0.35, 24, 48);
    glPopMatrix();

    // Cilindro — plateado
    // set_material(0.1f, 0.1f, 0.1f, 0.5f, 0.5f, 0.5f, 90.0f);
    // glPushMatrix();
    //     glTranslatef(1.5f, Y, -0.6f);
    //     glRotatef(-90.0f, 1, 0, 0);
    //     GLUquadric* q = gluNewQuadric();
    //     gluQuadricNormals(q, GLU_SMOOTH);
    //     gluCylinder(q, 0.28, 0.28, 0.6, 32, 8);
    //     gluDeleteQuadric(q);
    // glPopMatrix();

    dibujar_libros();
}