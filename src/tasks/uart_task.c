//Created by Bananums: https://github.com/Bananums

#include "uart_task.h"
#include "integration/task_data.h"
#include "nanners_protocol/nanners_protocol.h"


void uart_read_task(void *arg) {
    const ReadTaskParams *params = (ReadTaskParams *) arg;
    const uart_port_t uart_num = params->uart_num;
    SharedState *shared_state = params->shared_state;
    const size_t uart_buffer_size = 64;
    uint8_t uart_buffer[uart_buffer_size];

    DrivePayload drive_payload = {
        .heartbeat = 0,
        .steering = 0.0f,
        .throttle = 0.0f,
    };

    float steering = 0.0f;

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
        steering = steering + 0.025f;  //TODO implement actual use in ProcessMessage
        drive_payload.steering = steering;
        UpdateDriveState(shared_state, &drive_payload);
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
