#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;

const int LED_PIN_R = 4;
const int LED_PIN_G = 6;

SemaphoreHandle_t xSemaphore_r;

void led_1_task(void *p) {
  gpio_init(LED_PIN_R);
  gpio_set_dir(LED_PIN_R, GPIO_OUT);

  int delay = 250;

  while (true) {

    if (xSemaphoreTake(xSemaphore_r, pdMS_TO_TICKS(500)) == pdTRUE) {
      gpio_put(LED_PIN_R, 1);
      vTaskDelay(pdMS_TO_TICKS(delay));
      gpio_put(LED_PIN_R, 0);
      vTaskDelay(pdMS_TO_TICKS(delay));
    }
  }
}

void btn_1_task(void *p) {
  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  while (true) {
    if (!gpio_get(BTN_PIN_R)) {
      while (!gpio_get(BTN_PIN_R)) {
        vTaskDelay(pdMS_TO_TICKS(1));
      }
      xSemaphoreGive(xSemaphore_r);
    }
  }
}

int main() {
  stdio_init_all();
  printf("Start RTOS \n");

  xSemaphore_r = xSemaphoreCreateBinary();

  xTaskCreate(led_1_task, "LED_Task 1", 256, NULL, 1, NULL);
  xTaskCreate(btn_1_task, "BTN_Task 1", 256, NULL, 1, NULL);

  vTaskStartScheduler();

  while (true)
    ;
}
