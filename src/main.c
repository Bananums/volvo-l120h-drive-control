//Created by Bananums: https://github.com/Bananums

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "memory.h"

#include "nanners_protocol/nanners_protocol.h"
#include "integration/enums.h"
#include "integration/types.h"

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
    QueueHandle_t queue;
} ReadTaskParams;

typedef struct {
    uart_port_t uart_port;
    uint8_t test;
    QueueHandle_t queue;
} WriteTaskParams;

typedef struct {
    QueueHandle_t queue;
} MainTaskParams;

void uart_read_task(void *arg) {
    const ReadTaskParams params = *(ReadTaskParams *) arg;
    const uart_port_t uart_num = params.uart_num;
    const size_t uart_buffer_size = 64;
    uint8_t uart_buffer[uart_buffer_size];

    while (1) {
        const int len = uart_read_bytes(uart_num, &uart_buffer, uart_buffer_size, pdMS_TO_TICKS(10));
        if (len > 0) {
            printf("Received %d bytes\n", len);
            for (int i = 0; i < len; i++) {

                //Apparently declaration of primitives inside of loop has no performance affect.
                const uint8_t byte = uart_buffer[i];
                NannersProcessBytes(byte);
                NannersFrame frame;
                if (NannersGetFrame(&frame)) {
                    printf("Frame Ready for processing\n");
                    //ProcessMessage(frame.frame_id, frame.payload, frame.length);
                }
            }
        }
    }
}

void uart_write_task(void *arg) {
    const WriteTaskParams params = *(WriteTaskParams *) arg;
    uart_port_t uart_port = params.uart_port;
    QueueHandle_t queue = params.queue;
    uint8_t byte = 0;

    while (1) {
        if (xQueueReceive(queue, &byte, portMAX_DELAY) == pdTRUE) {
            uart_write_bytes(uart_port, &byte, sizeof(byte));
        }
        vTaskDelay(pdMS_TO_TICKS(10)); //TODO Se if it makes sense to sleep in addition to the blocking call
    }
}

void main_task(void *arg) {
    const MainTaskParams params = *(MainTaskParams *) arg;
    QueueHandle_t queue = params.queue;
    uint8_t byte = 0;
    while (1) {
        if (byte == 255) {
            byte = 0;
        }
        byte++;

        if (xQueueSend(queue, &byte, sizeof(byte)) == pdTRUE) {
            printf("Sending to queue: byte %d\n", byte);
        } else {
            printf("Failed to send to queue: byte %d\n", byte);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
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

    QueueHandle_t x_queue = xQueueCreate(10, sizeof(uint8_t));

    ReadTaskParams task_params = {
        .uart_num = uart_port,
        .buffer_size = buffer_size,
        //.queue = x_queue
    };

    WriteTaskParams write_task_params = {
        .uart_port = uart_port,
        .test = 200,
        .queue = x_queue
    };

    MainTaskParams main_task_params = {
        .queue = x_queue
    };


    printf("UART listening on GPIO RX=%d TX=%d...\n", RX_PIN, TX_PIN);
    xTaskCreate(main_task, "main_task", 4096, &main_task_params, 2, NULL);
    xTaskCreate(uart_read_task, "UART_RX_Task", 4096, &task_params, 5, NULL);
    xTaskCreate(uart_write_task, "UART_TX_Task", 4096, &write_task_params, 5, NULL);
}
