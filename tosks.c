#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "doglab.h"
#include "display.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define DEBOUNCE_TIME_MS 72

// Fila de processamento para 'pressadas'
QueueHandle_t presses_queue;

// Quantidade de vezes pressionado
int number_pressed = 0;

typedef struct
{
    uint led_pin;    // Pino do LED que será acendido
    uint button_pin; // Pino do botão correspondente
} t_led_button_map;

typedef struct t_led_params
{
    uint led_number;
    bool is_pressed;
} t_led_params;

bool debounce_button(uint button_pin)
{
    if (!gpio_get(button_pin))
    {
        vTaskDelay(pdMS_TO_TICKS(DEBOUNCE_TIME_MS));
        if (!gpio_get(button_pin))
        {
            return true;
        }
    }
    return false;
}

void setup(void)
{
    stdio_init_all();
    init_i2c();
    init_display();
    init_leds();
    init_buttons();
    clear_display();
}

void update_button_state(uint led_number, bool is_pressed)
{
    gpio_put(led_number, is_pressed); // Atualiza o LED para refletir o estado do botão
}

void vButtonStateTask(void *task_params)
{
    uint led_number = ((t_led_params *)task_params)->led_number;
    bool is_pressed;

    for (;;)
    {
        if (xQueueReceive(presses_queue, &is_pressed, portMAX_DELAY) == pdPASS)
        {
            if (is_pressed)
                number_pressed++;
            update_button_state(led_number, is_pressed);
        }
    }
}

void vPressRegisterTask(void *task_params)
{
    for (;;)
    {
        bool press = debounce_button(BTA);

        if (xQueueSend(presses_queue, &press, portMAX_DELAY) != pdPASS)
        {
            printf("Erro ao enviar para a fila\n");
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // Aguarda um pouco para evitar sobrecarga
    }
}

void vDisplayClickCountTask(void *task_params)
{
    for (;;)
    {
        clear_display();
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "Numero de cliques: %d", number_pressed);
        display_text(buffer);            // Exibe o número de cliques
        vTaskDelay(pdMS_TO_TICKS(1000)); // Aguarda 1 segundo
    }
}

int main()
{
    setup();

    presses_queue = xQueueCreate(5, sizeof(bool));
    if (presses_queue == NULL)
    {
        while (1)
        {
            printf("Erro ao criar a fila!\n");
        }
    }

    const uint blue_led = LED_BLUE;
    const uint green_led = LED_GREEN;

    t_led_params *task_prm_01 = (t_led_params *)malloc(sizeof(t_led_params));

    if (task_prm_01 == NULL)
    {
        while (1)
        {
            printf("Erro ao alocar memória para task_prm_01\n");
        }
    }

    task_prm_01->led_number = green_led;
    task_prm_01->is_pressed = false;

    // Criação das tarefas
    xTaskCreate(vPressRegisterTask, "Register ButtonPress", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vButtonStateTask, "Button State Task", 128, (void *)task_prm_01, 1, NULL);
    xTaskCreate(vDisplayClickCountTask, "Display Click Count", 128, NULL, 1, NULL);

    // Inicia o escalonador
    vTaskStartScheduler();

    // O código nunca deve chegar aqui se o escalonador estiver funcionando corretamente
    for (;;)
        ;
}
