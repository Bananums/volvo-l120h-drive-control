//Created by Bananums: https://github.com/Bananums

#include "main_task.h"

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
