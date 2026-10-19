#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define BTN_PIN_R 28
#define BTN_PIN_Y 21

#define LED_PIN_R 5
#define LED_PIN_Y 10

QueueHandle_t xQueueButtons;

void btn_callback(uint gpio, uint32_t events) {
    if (events & GPIO_IRQ_EDGE_FALL_MASK) {
        uint32_t button_pin = gpio;
        BaseType_t higherPriorityTaskWoken = pdFALSE;

        xQueueSendFromISR(
            xQueueButtons,
            &button_pin,
            &higherPriorityTaskWoken
        );

        portYIELD_FROM_ISR(higherPriorityTaskWoken);
    }
}

void btn_task(void *p) {
    bool blinking_r = false;
    bool blinking_y = false;
    bool level_r = false;
    bool level_y = false;

    bool pressed_before_r = false;
    bool pressed_before_y = false;
    TickType_t last_press_r = 0;
    TickType_t last_press_y = 0;

    const TickType_t debounce_time = pdMS_TO_TICKS(200);
    const TickType_t blink_period = pdMS_TO_TICKS(100);
    TickType_t last_wake = xTaskGetTickCount();

    while (true) {
        vTaskDelayUntil(&last_wake, blink_period);

        uint32_t button_pin;

        /* Processa todos os eventos recebidos desde a última atualização */
        while (xQueueReceive(xQueueButtons, &button_pin, 0) == pdTRUE) {
            TickType_t now = xTaskGetTickCount();

            if (button_pin == BTN_PIN_R) {
                if (!pressed_before_r ||
                    (now - last_press_r) >= debounce_time) {
                    blinking_r = !blinking_r;
                    last_press_r = now;
                    pressed_before_r = true;

                    if (!blinking_r) {
                        level_r = false;
                        gpio_put(LED_PIN_R, 0);
                    }
                }
            } else if (button_pin == BTN_PIN_Y) {
                if (!pressed_before_y ||
                    (now - last_press_y) >= debounce_time) {
                    blinking_y = !blinking_y;
                    last_press_y = now;
                    pressed_before_y = true;

                    if (!blinking_y) {
                        level_y = false;
                        gpio_put(LED_PIN_Y, 0);
                    }
                }
            }
        }

        if (blinking_r) {
            level_r = !level_r;
            gpio_put(LED_PIN_R, level_r);
        }

        if (blinking_y) {
            level_y = !level_y;
            gpio_put(LED_PIN_Y, level_y);
        }
    }
}

int main() {
    stdio_init_all();

    xQueueButtons = xQueueCreate(10, sizeof(uint32_t));

    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);
    gpio_put(LED_PIN_R, 0);

    gpio_init(LED_PIN_Y);
    gpio_set_dir(LED_PIN_Y, GPIO_OUT);
    gpio_put(LED_PIN_Y, 0);

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_init(BTN_PIN_Y);
    gpio_set_dir(BTN_PIN_Y, GPIO_IN);
    gpio_pull_up(BTN_PIN_Y);

    gpio_set_irq_enabled_with_callback(
        BTN_PIN_R,
        GPIO_IRQ_EDGE_FALL,
        true,
        &btn_callback
    );

    gpio_set_irq_enabled(BTN_PIN_Y, GPIO_IRQ_EDGE_FALL, true);

    xTaskCreate(btn_task, "BTN_Task", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true)
        ;
}