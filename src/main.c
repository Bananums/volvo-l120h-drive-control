#include <stdio.h>
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main() {
    const uint16_t UART_PORT = UART_NUM_0;
    const uint16_t BUF_SIZE = 1024;
    // Configure UART parameters
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_PORT, &uart_config);

    // Install UART driver
    uart_driver_install(UART_PORT, BUF_SIZE * 2, 0, 0, NULL, 0);

    uint8_t data[BUF_SIZE];

    while (1) {
        // Read data from UART0 (USB)
        int len = uart_read_bytes(UART_PORT, data, BUF_SIZE, pdMS_TO_TICKS(1000));
        if (len > 0) {
            // Print the received data to the console
            printf("Received: %.*s\n", len, data);

            // Echo the data back
            uart_write_bytes(UART_PORT, (const char *)data, len);
        }
    }
}
