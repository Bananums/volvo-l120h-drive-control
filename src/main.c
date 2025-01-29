//Created by Bananums: https://github.com/Bananums

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "memory.h"

#include "config_io.h"

void startup_led_blink() {
    bool led_on = true;
    for (int i = 0; i < 10; i++) {
        gpio_set_level(LED_PIN, led_on);
        vTaskDelay(pdMS_TO_TICKS(250));
        led_on = !led_on;
    }
    gpio_set_level(LED_PIN, false);
}

typedef struct {
    uart_port_t uart_num;
    size_t buffer_size;
} ReadTaskParams;

void uart_read_task(void *arg) {
    ReadTaskParams params = *(ReadTaskParams *)arg;
    uart_port_t uart_num = params.uart_num;
    size_t buffer_size = params.buffer_size;

    uint8_t data[buffer_size];

    while (1) {
        int len = uart_read_bytes(uart_num, data, buffer_size, pdMS_TO_TICKS(10));
        if (len > 0) {
            printf("Received: %.*s\n", len, data);
            printf("Hex Dump: ");
            for (int i = 0; i < len; i++) {
                printf("%02X ", data[i]);
            }
            printf("\n");
        }
    }
}

void app_main() {
    const uart_port_t uart_port = UART_NUM_2;
    const gpio_num_t TX_PIN = GPIO_NUM_17;
    const gpio_num_t RX_PIN = GPIO_NUM_16;
    const size_t buffer_size = 1024;

    static const uart_config_t UART_CONF = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    // Configure GPIO
    gpio_config(&ONBOARD_LED_GPIO_CONF);

    // Configure UART
    uart_param_config(uart_port, &UART_CONF);
    uart_driver_install(uart_port, buffer_size * 2, 0, 0, NULL, 0);
    uart_set_pin(uart_port, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    startup_led_blink();

    // Allocate memory for struct and set parameters
    ReadTaskParams task_params = {
    .uart_num = uart_port,
    .buffer_size = buffer_size
    };

    printf("UART listening on GPIO RX=%d TX=%d...\n", RX_PIN, TX_PIN);
    xTaskCreate(uart_read_task, "UART_RX_Task", 4096, &task_params, 5, NULL);
}
