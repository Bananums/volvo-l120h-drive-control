//Created by Bananums: https://github.com/Bananums

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "memory.h"

#include "nanners_protocol/nanners_protocol.h"
#include "integration/enums.h"
#include "integration/types.h"

#include "config_io.h"

typedef struct __attribute__((packed)) {
    uint8_t heartbeat;
    uint8_t function_request;
    float steering;
    float throttle;
    float tilt;
    float lift;
} CommandPayload;


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

    size_t uart_buffer_size = 64;
    uint8_t uart_buffer[uart_buffer_size];

    CommandPayload payload;
    uint8_t byte;
    while (1) {

        const int len = uart_read_bytes(uart_num, &uart_buffer, uart_buffer_size, pdMS_TO_TICKS(10));
        if (len > 0) { //TODO add ringbuffer apporach. instead of reading one byte each 10ms.
            printf("Received %d bytes\n", len);
            for (int i = 0; i < len; i++) {
                byte = uart_buffer[i];
                NannersProcessBytes(byte);
                NannersFrame frame;
                if (NannersGetFrame(&frame)) {
                    printf("Frame Ready for processing\n");
                    //ProcessMessage(frame.frame_id, frame.payload, frame.length);l
                }
            }
        }
    }
}

void app_main() {
    static_assert(sizeof(DrivePayload) <= kMaxPayloadSize, "DrivePayload too large!");
    static_assert(sizeof(ToolPayload) <= kMaxPayloadSize, "ToolPayload too large!");
    static_assert(sizeof(FunctionPayload) <= kMaxPayloadSize, "FunctionPayload too large!");
    static_assert(sizeof(FeedbackPayload) <= kMaxPayloadSize, "FeedbackPayload too large!");

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
