#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "objetos.h"

void dibujar_ambiental()
{
    GLfloat mat_ambient[]  = {0.6f, 0.0f, 0.6f, 1.0f};
    GLfloat mat_diffuse[]  = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat mat_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialf (GL_FRONT, GL_SHININESS, 0.0);

    glPushMatrix();
    glTranslatef(-3.5, 0, 0);
    glutSolidSphere(1.0, 32, 32);
    glPopMatrix();
}

void dibujar_difusa()
{
    GLfloat mat_ambient[]  = {0.1f, 0.3f, 0.1f, 1.0f};
    GLfloat mat_diffuse[]  = {0.2f, 0.9f, 0.2f, 1.0f};
    GLfloat mat_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialf (GL_FRONT, GL_SHININESS, 0.0);

    glPushMatrix();
    glTranslatef(0, -0.5, 0);
    glutSolidTeapot(1.0);
    glPopMatrix();
}

void dibujar_especular()
{
    GLfloat mat_ambient[]  = {0.1f, 0.1f, 0.3f, 1.0f};
    GLfloat mat_diffuse[]  = {0.2f, 0.2f, 0.8f, 1.0f};
    GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialf (GL_FRONT, GL_SHININESS, 96.0);

    glPushMatrix();
    glTranslatef(3.5, 0, 0);
    glutSolidSphere(1.0, 32, 32);
    glPopMatrix();
}