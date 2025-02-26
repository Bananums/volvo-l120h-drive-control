//Created by Bananums: https://github.com/Bananums

#ifndef ENUMS_H
#define ENUMS_H

#include <stdint.h>

typedef enum {
    kFrameDrive = 1001,
    kFrameTool = 1002,
    kFrameFunction = 1003
} FrameId;

typedef enum {
    kNone = 0,
    kGearDrive = 1,
    kGearNeutral = 2,
    kGearReverse = 3,
    kSafeStop = 4
} Function;

#endif //ENUMS_H
