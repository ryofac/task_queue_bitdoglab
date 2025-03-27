#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <string.h>
#include "ssd1306.h"
#include "doglab.h"

// Definições e Variáveis Globais do Display
extern int x_pos_text;
extern int y_pos_text;
extern int SCREEN_BOTTOM_LIMIT;
extern char *bottom_screen_text;
extern ssd1306_t display;

// Protótipos das Funções
void clear_display();
void display_text_with_scale(char *text, int scale);
void display_text(char *text);
void draw_screen_botton();
void draw_screen_components();

#endif