/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>


#include <stdio.h>
#include <string.h> 
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/irq.h"

const int BTN_PIN_R = 28;
const int BTN_PIN_Y = 21;

const int LED_PIN_R = 5;
const int LED_PIN_Y = 10;


void btn_task(void* p) {

    while (true) {

    }
}



int main() {
    stdio_init_all();

    xTaskCreate(btn_task, "BTN_Task 1", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while(1){}

    return 0;
}