//Created by Bananums: https://github.com/Bananums

#ifndef UART_READ_TASK_H
#define UART_READ_TASK_H

#include "freertos/FreeRTOS.h"
#include "driver/uart.h"
#include "tasks/task_params.h"

#include "nanners_protocol/nanners_protocol.h"

void uart_read_task(void *arg);
void uart_write_task(void *arg);

void ProcessMessage(SharedState *shared_state, const NannersFrame *nanners_frame);


#endif //UART_READ_TASK_H
