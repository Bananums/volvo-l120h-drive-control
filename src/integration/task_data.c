//Created by Bananums: https://github.com/Bananums

#include "freertos/FreeRTOS.h"
#include "task_data.h"

void UpdateDriveState(SharedState *state, DrivePayload *drive){
    if(xSemaphoreTake(state->semaphore_mutex, portMAX_DELAY) == pdTRUE){
      state->drive = *drive;
      xSemaphoreGive(state->semaphore_mutex);
    }
}

void UpdateToolState(SharedState *state, ToolPayload *tool){
    if(xSemaphoreTake(state->semaphore_mutex, portMAX_DELAY) == pdTRUE){
        state->tool = *tool;
        xSemaphoreGive(state->semaphore_mutex);
    }
}

void UpdateFunction(SharedState *state, FunctionPayload *function){
    if(xSemaphoreTake(state->semaphore_mutex, portMAX_DELAY) == pdTRUE){
        state-> function = *function;
        xSemaphoreGive(state->semaphore_mutex);
    }
}

void ReadDriveState(SharedState *state, DrivePayload *drive){
    if(xSemaphoreTake(state->semaphore_mutex, portMAX_DELAY) == pdTRUE){
        *drive = state->drive;
        xSemaphoreGive(state->semaphore_mutex);
    }
}

void ReadToolState(SharedState *state, ToolPayload *tool){
    if(xSemaphoreTake(state->semaphore_mutex, portMAX_DELAY) == pdTRUE){
        *tool = state->tool;
        xSemaphoreGive(state->semaphore_mutex);
    }
}

void ReadFunction(SharedState *state, FunctionPayload *function){
    if(xSemaphoreTake(state->semaphore_mutex, portMAX_DELAY) == pdTRUE){
        *function = state->function;
        xSemaphoreGive(state->semaphore_mutex);
    }
}
