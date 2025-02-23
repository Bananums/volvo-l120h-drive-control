//Created by Bananums: https://github.com/Bananums

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "memory.h"

#include "utils/startup_checks.h"  //Used for compile time asserts. Even though it is informed to not be in use.

#include "config_io.h"
#include "nanners_protocol/nanners_protocol.h"
#include "tasks/main_task.h"
#include "tasks/uart_task.h"

void StartupLedBlink() {
    gpio_config(&ONBOARD_LED_GPIO_CONF);
    bool led_on = true;
    for (int i = 0; i < 10; i++) {
        gpio_set_level(kLedPin, led_on);
        vTaskDelay(pdMS_TO_TICKS(250));
        led_on = !led_on;
    }
    gpio_set_level(kLedPin, false);
}

void InitSharedState(SharedState *state) {
    state->semaphore_mutex = xSemaphoreCreateMutex();
}

void StartTasks() {
    const uart_port_t uart_port = UART_NUM_2;
    const gpio_num_t TX_PIN = GPIO_NUM_17;
    const gpio_num_t RX_PIN = GPIO_NUM_16;
    const int16_t buffer_size = 1024;
    QueueHandle_t x_queue = xQueueCreate(10, sizeof(uint8_t));

    // Configure UART
    uart_param_config(uart_port, &UART_CONF);
    uart_driver_install(uart_port, buffer_size * 2, 0, 0, NULL, 0);
    uart_set_pin(uart_port, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    static SharedState shared_state; // Must be static in order to still exist for tasks when function returns
    InitSharedState(&shared_state);

    ReadTaskParams task_params = {
        .uart_num = uart_port,
        .buffer_size = buffer_size,
        .shared_state = &shared_state
    };

    WriteTaskParams write_task_params = {
        .uart_port = uart_port,
        .queue = x_queue
    };

    MainTaskParams main_task_params = {
        .queue = x_queue,
        .shared_state = &shared_state
    };

    xTaskCreate(main_task, "main_task", 4096, &main_task_params, 2, NULL);
    xTaskCreate(uart_read_task, "UART_RX_Task", 4096, &task_params, 5, NULL);
    xTaskCreate(uart_write_task, "UART_TX_Task", 4096, &write_task_params, 5, NULL);
}

void app_main() {
    StartupLedBlink();
    StartTasks();
}
