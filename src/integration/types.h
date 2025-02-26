//Created by Bananums: https://github.com/Bananums

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint8_t heartbeat;
    float steering;
    float throttle;
} DrivePayload;

typedef struct __attribute__((packed)) {
    uint8_t heartbeat;
    float tilt;
    float lift;
} ToolPayload;

typedef struct __attribute__((packed)) {
    uint8_t function;
} FunctionPayload;

typedef struct __attribute__((packed)) {
    float steering;
    float throttle;
    float tilt;
    float lift;
    int8_t gear;
} FeedbackPayload;

typedef struct __attribute__((packed)) {
    DrivePayload drive;
    ToolPayload tool;
    FunctionPayload function;
    SemaphoreHandle_t semaphore_mutex;
} SharedState;

#endif //TYPES_H
