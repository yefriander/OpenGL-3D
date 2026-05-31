#include <stdio.h>
#include "texturas.h"

// ---------------------------------------------------------------------------
// Carga un archivo BMP 24-bit y lo sube a la GPU como textura OpenGL.
//
// Detalles importantes que se explican en clase:
//   - BMP guarda los píxeles en orden BGR (no RGB) → hay que intercambiar R y B
//   - BMP guarda filas de abajo hacia arriba → OpenGL lo invierte solo con glTexImage2D
//   - Cada fila del BMP está rellena hasta múltiplo de 4 bytes (padding)
//   - El tamaño de la imagen DEBE ser potencia de 2: 64, 128, 256, 512, 1024...
// ---------------------------------------------------------------------------
GLuint cargar_textura(const char* archivo)
{
    FILE* f = fopen(archivo, "rb");
    if (!f) {
        printf("ERROR: no se puede abrir '%s'\n", archivo);
        return 0;
    }

    // Leer los primeros 54 bytes: cabecera BMP
    unsigned char cab[54];
    if (fread(cab, 1, 54, f) != 54 || cab[0] != 'B' || cab[1] != 'M') {
        printf("ERROR: '%s' no es un BMP válido\n", archivo);
        fclose(f); return 0;
    }

    int offset = *(int*)&cab[10];  // byte donde empiezan los píxeles
    int ancho  = *(int*)&cab[18];
    int alto   = *(int*)&cab[22];

    // Cada fila ocupa (ancho*3) bytes pero rellena hasta múltiplo de 4
    int tamFila = ((ancho * 3 + 3) / 4) * 4;

    fseek(f, offset, SEEK_SET);
    unsigned char* raw = new unsigned char[tamFila * alto];
    fread(raw, 1, tamFila * alto, f);
    fclose(f);

    // Convertir BGR→RGB y quitar el padding de fila
    unsigned char* pixeles = new unsigned char[ancho * alto * 3];
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            int src = y * tamFila + x * 3;
            int dst = y * ancho  * 3 + x * 3;
            pixeles[dst]   = raw[src + 2]; // R
            pixeles[dst+1] = raw[src + 1]; // G
            pixeles[dst+2] = raw[src];     // B
        }
    }
    delete[] raw;

    // Crear la textura en OpenGL
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);      // repetir en X
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);      // repetir en Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // filtro al alejar
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // filtro al acercar

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, pixeles);

    delete[] pixeles;
    printf("Textura '%s' cargada (%dx%d) → ID=%u\n", archivo, ancho, alto, id);
    return id;
}

// ---------------------------------------------------------------------------
// Helpers internos
// ---------------------------------------------------------------------------
static void activar_textura(GLuint tex)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);

    // GL_MODULATE: multiplica el color de la textura por la iluminación OpenGL
    // (alternativa: GL_REPLACE ignora la luz y muestra la textura pura)
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Material blanco para que la textura muestre su color real bajo la luz
    GLfloat blanco[]  = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat spec[]    = {0.3f, 0.3f, 0.3f, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blanco);
    glMaterialfv(GL_FRONT, GL_SPECULAR,            spec);
    glMaterialf (GL_FRONT, GL_SHININESS,           20.0f);
}

static void desactivar_textura()
{
    glDisable(GL_TEXTURE_2D);
}

// ---------------------------------------------------------------------------
// Cubo unitario (lado = 1) centrado en el origen.
// Cada cara mapea la textura completa (0,0)→(1,1).
// glNormal3f le dice a la luz cómo iluminar cada cara.
// ---------------------------------------------------------------------------
void dibujar_cubo_texturizado(GLuint tex)
{
    activar_textura(tex);
    glBegin(GL_QUADS);

    // Cara frontal (z = +0.5)
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0); glVertex3f(-0.5f, -0.5f,  0.5f);
    glTexCoord2f(1,0); glVertex3f( 0.5f, -0.5f,  0.5f);
    glTexCoord2f(1,1); glVertex3f( 0.5f,  0.5f,  0.5f);
    glTexCoord2f(0,1); glVertex3f(-0.5f,  0.5f,  0.5f);

    // Cara trasera (z = -0.5)
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0); glVertex3f( 0.5f, -0.5f, -0.5f);
    glTexCoord2f(1,0); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1,1); glVertex3f(-0.5f,  0.5f, -0.5f);
    glTexCoord2f(0,1); glVertex3f( 0.5f,  0.5f, -0.5f);

    // Cara superior (y = +0.5)
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0); glVertex3f(-0.5f,  0.5f,  0.5f);
    glTexCoord2f(1,0); glVertex3f( 0.5f,  0.5f,  0.5f);
    glTexCoord2f(1,1); glVertex3f( 0.5f,  0.5f, -0.5f);
    glTexCoord2f(0,1); glVertex3f(-0.5f,  0.5f, -0.5f);

    // Cara inferior (y = -0.5)
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1,0); glVertex3f( 0.5f, -0.5f, -0.5f);
    glTexCoord2f(1,1); glVertex3f( 0.5f, -0.5f,  0.5f);
    glTexCoord2f(0,1); glVertex3f(-0.5f, -0.5f,  0.5f);

    // Cara derecha (x = +0.5)
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0); glVertex3f( 0.5f, -0.5f,  0.5f);
    glTexCoord2f(1,0); glVertex3f( 0.5f, -0.5f, -0.5f);
    glTexCoord2f(1,1); glVertex3f( 0.5f,  0.5f, -0.5f);
    glTexCoord2f(0,1); glVertex3f( 0.5f,  0.5f,  0.5f);

    // Cara izquierda (x = -0.5)
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1,0); glVertex3f(-0.5f, -0.5f,  0.5f);
    glTexCoord2f(1,1); glVertex3f(-0.5f,  0.5f,  0.5f);
    glTexCoord2f(0,1); glVertex3f(-0.5f,  0.5f, -0.5f);

    glEnd();
    desactivar_textura();
}

// ---------------------------------------------------------------------------
// Pared: quad vertical, centrado en X, base en Y=0.
// 'repeticiones' controla cuántas veces se repite la textura (tiling).
// ---------------------------------------------------------------------------
void dibujar_pared_texturizada(GLuint tex, float ancho, float alto, float repeticiones)
{
    float r = repeticiones;
    activar_textura(tex);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glTexCoord2f(0, 0); glVertex3f(-ancho/2, 0,    0);
    glTexCoord2f(r, 0); glVertex3f( ancho/2, 0,    0);
    glTexCoord2f(r, r); glVertex3f( ancho/2, alto, 0);
    glTexCoord2f(0, r); glVertex3f(-ancho/2, alto, 0);
    glEnd();
    desactivar_textura();
}

// ---------------------------------------------------------------------------
// Piso: quad horizontal centrado en el origen, sobre Y=0.
// Con repeticiones=4 la textura se repite 4 veces en cada eje.
// ---------------------------------------------------------------------------
void dibujar_piso_texturizado(GLuint tex, float ancho, float profundo, float repeticiones)
{
    float r = repeticiones;
    activar_textura(tex);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0); glVertex3f(-ancho/2,   0, -profundo/2);
    glTexCoord2f(r, 0); glVertex3f( ancho/2,   0, -profundo/2);
    glTexCoord2f(r, r); glVertex3f( ancho/2,   0,  profundo/2);
    glTexCoord2f(0, r); glVertex3f(-ancho/2,   0,  profundo/2);
    glEnd();
    desactivar_textura();
}