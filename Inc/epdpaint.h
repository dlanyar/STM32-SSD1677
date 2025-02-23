#ifndef EPDPAINT_H
#define EPDPAINT_H

#include <stdint.h>

// Définition des constantes pour la rotation
#define ROTATE_0 0
#define ROTATE_90 1
#define ROTATE_180 2
#define ROTATE_270 3

// Définition de la structure Paint
typedef struct {
    uint8_t *image;
    int width;
    int height;
    int rotate;
} Paint;

// Color inverse. 1 or 0 = set or reset a bit if set a colored pixel
#define IF_INVERT_COLOR     1

#include "fonts.h"

// Fonctions pour manipuler la structure Paint
void Paint_Init(Paint *paint, uint8_t *image, int width, int height);
void Paint_Clear(Paint *paint, int colored);
void Paint_DrawAbsolutePixel(Paint *paint, int x, int y, int colored);
void Paint_DrawPixel(Paint *paint, int x, int y, int colored);
void Paint_DrawCharAt(Paint *paint, int x, int y, char ascii_char,struct sFONT *font, int colored);
void Paint_DrawStringAt(Paint *paint, int x, int y, const char *text,struct sFONT *font, int colored);
void Paint_DrawLine(Paint *paint, int x0, int y0, int x1, int y1, int colored);
void Paint_DrawHorizontalLine(Paint *paint, int x, int y, int line_width, int colored);
void Paint_DrawVerticalLine(Paint *paint, int x, int y, int line_height, int colored);
void Paint_DrawRectangle(Paint *paint, int x0, int y0, int x1, int y1, int colored);
void Paint_DrawFilledRectangle(Paint *paint, int x0, int y0, int x1, int y1, int colored);
void Paint_DrawCircle(Paint *paint, int x, int y, int radius, int colored);
void Paint_DrawFilledCircle(Paint *paint, int x, int y, int radius, int colored);

// Getters et Setters
uint8_t *Paint_GetImage(Paint *paint);
int Paint_GetWidth(Paint *paint);
void Paint_SetWidth(Paint *paint, int width);
int Paint_GetHeight(Paint *paint);
void Paint_SetHeight(Paint *paint, int height);
int Paint_GetRotate(Paint *paint);
void Paint_SetRotate(Paint *paint, int rotate);

#endif // EPDPAINT_H
