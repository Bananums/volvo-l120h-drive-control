//Created by Bananums: https://github.com/Bananums

#include "tasks/uart_task.h"

#include <string.h>

#include "integration/task_data.h"
#include "integration/enums.h"

void uart_read_task(void *arg) {
    const ReadTaskParams *params = (ReadTaskParams *) arg;
    const uart_port_t uart_num = params->uart_num;
    SharedState *shared_state = params->shared_state;
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
                    ProcessMessage(shared_state, &frame);
                }
            }
        }
    }
}

void ProcessMessage(SharedState *shared_state, const NannersFrame *nanners_frame) {
    switch (nanners_frame->frame_id) {
        case kFrameDrive: {
            DrivePayload drive_payload;
            memcpy(&drive_payload, nanners_frame->payload, sizeof(drive_payload));
            UpdateDriveState(shared_state, &drive_payload);
            break;
        }
        case kFrameTool: {
            ToolPayload tool_payload;
            memcpy(&tool_payload, nanners_frame->payload, sizeof(tool_payload));
            UpdateToolState(shared_state, &tool_payload);
            break;
        }
        case kFrameFunction: {
            FunctionPayload function_payload;
            memcpy(&function_payload, nanners_frame->payload, sizeof(function_payload));
            UpdateFunction(shared_state, &function_payload);
            break;
        }
        default: {
            break;
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
