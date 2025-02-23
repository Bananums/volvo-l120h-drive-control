//Created by Bananums: https://github.com/Bananums

#ifndef MAIN_TASK_H
#define MAIN_TASK_H


#include "freertos/FreeRTOS.h"
#include "driver/uart.h"

typedef struct {
    QueueHandle_t queue;
} MainTaskParams;

void main_task(void *arg);

#endif //MAIN_TASK_H
