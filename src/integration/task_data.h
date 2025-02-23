//Created by Bananums: https://github.com/Bananums

#ifndef TASK_DATA_H
#define TASK_DATA_H

#include "types.h"

void UpdateDriveState(SharedState *state, DrivePayload *drive);
void UpdateToolState(SharedState *state, ToolPayload *tool);
void UpdateFunction(SharedState *state, FunctionPayload *function);

void ReadDriveState(SharedState *state, DrivePayload *drive);
void ReadToolState(SharedState *state, ToolPayload *tool);
void ReadFunction(SharedState *state, FunctionPayload *function);

#endif //TASK_DATA_H
