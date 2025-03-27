#include "display.h"
#include "pico/stdlib.h"
#include "doglab.h"

// Variáveis do display (inicialização feita em outro lugar no código principal)
int x_pos_text = 0;
int y_pos_text = 0;
int SCREEN_BOTTOM_LIMIT = SCREEN_HEIGHT - 8;
char *bottom_screen_text = "Task Queue";

/**
 * @brief Limpa o display OLED.
 *
 * Chama a função `ssd1306_clear` para apagar todos os pixels do display e redesenha os componentes
 */
void clear_display()
{
  ssd1306_clear(&display);

  // Reseta variáveis do texto
  x_pos_text = 0;
  y_pos_text = 0;

  draw_screen_components();
}

/**
 * @brief Exibe texto no display OLED com uma escala específica.
 *
 * @param text O texto a ser exibido.
 * @param scale O fator de escala do texto (1 = normal, 2 = dobro, etc.).
 */
void display_text_with_scale(char *text, int scale)
{
  int char_width = scale * 6;
  int text_width = strlen(text) * char_width;
  if (x_pos_text + text_width > SCREEN_WIDTH)
  {
    x_pos_text = 0;
    y_pos_text += 8;

    if (y_pos_text + 8 > SCREEN_BOTTOM_LIMIT)
    {
      y_pos_text = 0;
      clear_display();
    }
  }

  ssd1306_draw_string(&display, x_pos_text, y_pos_text, 1, text);
  ssd1306_show(&display);
  x_pos_text += text_width;
}

/**
 * @brief Exibe texto no display OLED (usando escala 1).
 *  Chama a função `display_text_with_scale` com a escala padrão (1).
 * @param text O texto a ser exibido.
 */
void display_text(char *text)
{
  display_text_with_scale(text, 1);
}

/**
 @brief Desenha a parte inferior (estática) da tela do OLED.
 */
void draw_screen_botton()
{
  int text_len = strlen(bottom_screen_text);
  ssd1306_draw_string(&display, (SCREEN_WIDTH - text_len * 6) - 2, SCREEN_BOTTOM_LIMIT - 16 / 2, 1, bottom_screen_text);
  ssd1306_draw_square(&display, 0, SCREEN_BOTTOM_LIMIT, SCREEN_WIDTH, SCREEN_HEIGHT - SCREEN_BOTTOM_LIMIT);
  ssd1306_show(&display);
}

/**
 @brief Desenha todos os componentes estáticos da tela.
 */
void draw_screen_components()
{
  draw_screen_botton();
}