#ifndef MOUSE_H
#define MOUSE_H

extern float radio;
extern float yaw;
extern float pitch;

void mouse_boton(int boton, int estado, int x, int y);
void mouse_movimiento(int x, int y);
void mouse_scroll(int wheel, int direccion, int x, int y);

#endif