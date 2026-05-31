#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include "mouse.h"

float radio = 12.0;
float yaw   = 45 * M_PI / 180.0f;
float pitch = 30 * M_PI / 180.0f;

static bool  mouse_presionado = false;
static float mouse_x_ant     = 0;
static float mouse_y_ant     = 0;
static float sensibilidad    = 0.005f;

void mouse_boton(int boton, int estado, int x, int y)
{
    if (boton == GLUT_LEFT_BUTTON) {
        mouse_presionado = (estado == GLUT_DOWN);
        mouse_x_ant = x;
        mouse_y_ant = y;
    }
}

void mouse_movimiento(int x, int y)
{
    if (!mouse_presionado)
        return;

    int delta_x = x - mouse_x_ant;
    int delta_y = y - mouse_y_ant;

    yaw   += delta_x * sensibilidad;
    pitch -= delta_y * sensibilidad;

    float limite = 89.0f * M_PI / 180.0f;
    if (pitch >  limite) pitch =  limite;
    if (pitch < -limite) pitch = -limite;

    mouse_x_ant = x;
    mouse_y_ant = y;

    glutPostRedisplay();
}