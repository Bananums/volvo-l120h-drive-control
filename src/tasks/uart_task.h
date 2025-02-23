//Created by Bananums: https://github.com/Bananums

#ifndef UART_READ_TASK_H
#define UART_READ_TASK_H

#include "freertos/FreeRTOS.h"
#include "driver/uart.h"

// Struct for passing parameters to uart_read_task
typedef struct {
    uart_port_t uart_num;
    size_t buffer_size;
    QueueHandle_t queue;
} ReadTaskParams;

// Struct for passing parameters to uart_write_task
typedef struct {
    uart_port_t uart_port;
    uint8_t test;
    QueueHandle_t queue;
} WriteTaskParams;

void uart_read_task(void *arg);
void uart_write_task(void *arg);


#endif //UART_READ_TASK_H
