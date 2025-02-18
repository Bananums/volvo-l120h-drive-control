//Created by Bananums: https://github.com/Bananums

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "memory.h"

#include "config_io.h"

typedef struct __attribute__((packed)) {
    uint8_t heartbeat;
    uint8_t function_request;
    float steering;
    float throttle;
    float tilt;
    float lift;
} CommandPayload;

typedef struct __attribute__((packed)) {
    uint8_t heartbeat;
    float steering;
    float throttle;
} DrivePayload;


typedef struct __attribute__((packed)) {
    uint8_t heartbeat;
    float tilt;
    float lift;
} ToolPayload;


typedef struct __attribute__((packed)) {
    uint8_t heartbeat;
    uint8_t function_request;
} FunctionPayload;

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
    ReadTaskParams params = *(ReadTaskParams *) arg;
    uart_port_t uart_num = params.uart_num;
    //size_t buffer_size = params.buffer_size;
    size_t payload_size = sizeof(CommandPayload);
    uint8_t data[payload_size];
    size_t bytes_received = 0;

    CommandPayload payload;
    while (1) {
        int len = uart_read_bytes(
            uart_num,
            data + bytes_received,
            payload_size - bytes_received,
            pdMS_TO_TICKS(10)
        );
        if (len > 0) {
            bytes_received += len;
            if (bytes_received == payload_size) {

                printf("Hex Dump: ");
                for (int i = 0; i < payload_size; i++) {
                    printf("%02X ", data[i]);
                }
                printf("\n");
                memcpy(&payload, data, payload_size); // Copy data into struct
                printf("Received CommandPayload:\n");
                printf("Heartbeat: %d\n", payload.heartbeat);
                printf("Function Request: %d\n", payload.function_request);
                printf("Steering: %.2f\n", payload.steering);
                printf("Throttle: %.2f\n", payload.throttle);
                printf("Tilt: %.2f\n", payload.tilt);
                printf("Lift: %.2f\n", payload.lift);
                bytes_received = 0;
            }
        }
    }
}

void app_main() {
    const uart_port_t uart_port = UART_NUM_2;
    const gpio_num_t TX_PIN = GPIO_NUM_17;
    const gpio_num_t RX_PIN = GPIO_NUM_16;
    const int16_t buffer_size = 1024;

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
