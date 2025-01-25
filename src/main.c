//Created by Bananums: https://github.com/Bananums

#include <stdio.h>
#include "config_io.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main() {

    static const uart_config_t UART_CONF = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    const uint16_t TX_PIN = GPIO_NUM_17;
    const uint16_t RX_PIN = GPIO_NUM_16;
    const uint16_t UART_PORT_2 = UART_NUM_2;
    const uint16_t BUF_SIZE = 1024;
    uart_param_config(UART_PORT_2, &UART_CONF);
    gpio_config(&ONBOARD_LED_GPIO_CONF);


    uart_driver_install(UART_PORT_2, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_set_pin(UART_PORT_2, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    uint8_t data[BUF_SIZE];

    bool led_on = true;
    for (int i = 0; i < 10; i++) {
        gpio_set_level(LED_PIN, led_on);
        vTaskDelay(pdMS_TO_TICKS(250));
        led_on = !led_on;
    }
    gpio_set_level(LED_PIN, false);

    printf("UART listening on GPIO RX=%d TX=%d...\n", RX_PIN, TX_PIN);

    while (1) {
        // Read data from UART2
        int len = uart_read_bytes(UART_PORT_2, data, BUF_SIZE, pdMS_TO_TICKS(1000));
        if (len > 0) {
            // Print the received data in both string and hex format
            printf("Received: %.*s\n", len, data);
            printf("Received %d bytes: %.*s\n", len, len, data);
            printf("Hex Dump: ");
            for (int i = 0; i < len; i++) {
                printf("%02X ", data[i]);
            }
            printf("\n");
        }
    }
}
