//
// Created by sondre on 2/23/25.
//

#ifndef TASK_PARAMS_H
#define TASK_PARAMS_H

#include "integration/types.h"

typedef struct {
    QueueHandle_t queue;
    SharedState *shared_state;
} MainTaskParams;

// Struct for passing parameters to uart_read_task
typedef struct {
    uart_port_t uart_num;
    size_t buffer_size;
    SharedState *shared_state;
} ReadTaskParams;

// Struct for passing parameters to uart_write_task
typedef struct {
    uart_port_t uart_port;
    QueueHandle_t queue;
} WriteTaskParams;

#endif //TASK_PARAMS_H
