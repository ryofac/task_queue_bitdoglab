#include <stdlib.h>
#include <stdio.h>

#include "doglab.h"
#include "ssd1306.h"
#include "hardware/adc.h"

// Aloca a variável do display da tela para ser manipulada pelas funções de display
ssd1306_t display;

void init_i2c()
{
  // Inicializa I2C no canal 1
  i2c_init(i2c1, 400 * 1000);
  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
  gpio_pull_up(I2C_SDA);
  gpio_pull_up(I2C_SCL);
}

void init_display()
{
  // Inicializa o display
  if (!ssd1306_init(&display, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_ADDRESS, i2c1))
  {
    printf("Display não iniciado");
  }
  else
  {
    printf("Display SSD1306 iniciado com sucesso!");
  }
}

void init_leds()
{
  int leds[3] = {LED_RED, LED_GREEN, LED_BLUE};
  for (int i = 0; i < 3; i++)
  {
    gpio_init(leds[i]);
    gpio_set_dir(leds[i], GPIO_OUT);
    gpio_put(leds[i], 0);
  }
}

void init_buzzer()
{
  gpio_init(BUZZER_PIN_1);
  gpio_set_dir(BUZZER_PIN_1, GPIO_OUT);
}

typedef void (*callback_t)(uint gpio, uint32_t events);
void initButtons(callback_t callbackFunction)
{
  if (callbackFunction == NULL)
    return;
  int buttons[2] = {BTA, BTB};
  for (int i = 0; i < 2; i++)
  {
    gpio_init(buttons[i]);
    gpio_set_dir(buttons[i], GPIO_IN);
    gpio_pull_up(buttons[i]);
    gpio_set_irq_enabled_with_callback(buttons[i], GPIO_IRQ_EDGE_RISE, true, callbackFunction);
  }
}

void init_analog()
{
  adc_init();
  adc_gpio_init(ANALOG_X);
  adc_gpio_init(ANALOG_Y);
  gpio_init(ANALOG_BTN);
  gpio_set_dir(ANALOG_BTN, GPIO_IN);
  gpio_pull_up(ANALOG_BTN);
}

void init_adc()
{
  adc_init();
}

void init_buttons()
{
  gpio_set_dir(BTA, GPIO_IN);
  gpio_set_dir(BTB, GPIO_IN);

  gpio_pull_up(BTA);
  gpio_pull_up(BTB);
}

void init_all()
{
  stdio_init_all();
  init_i2c();
  init_display();
  initButtons(0);
  init_leds();
  init_adc();
}
