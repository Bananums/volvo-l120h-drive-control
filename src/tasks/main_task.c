//Created by Bananums: https://github.com/Bananums

#include "main_task.h"
#include "integration/task_data.h"

void main_task(void *arg) {
    const MainTaskParams *params = (MainTaskParams *) arg;
    QueueHandle_t queue = params->queue;
    SharedState *shared_state = params->shared_state;
    DrivePayload drive_payload = {
    .heartbeat = 0,
    .steering = 0.0f,
    .throttle = 0.0f,
    };

    uint8_t byte = 0;
    while (1) {
        if (byte == 255) {
            byte = 0;
        }
        byte++;

        ReadDriveState(shared_state, &drive_payload);
        const float steering = drive_payload.steering;
        printf("Got drive.steering %f\n", steering);

        if (xQueueSend(queue, &byte, portMAX_DELAY) == pdTRUE) {  //TODO look into portMAX_DELAY
            printf("Sending to queue: byte %d\n", byte);
        } else {
            printf("Failed to send to queue: byte %d\n", byte);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
