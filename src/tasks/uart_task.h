//Created by Bananums: https://github.com/Bananums

#ifndef UART_READ_TASK_H
#define UART_READ_TASK_H

#include "freertos/FreeRTOS.h"
#include "driver/uart.h"
#include "task_params.h"



void uart_read_task(void *arg);
void uart_write_task(void *arg);


#endif //UART_READ_TASK_H
