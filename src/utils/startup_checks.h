//Created by Bananums: https://github.com/Bananums

#ifndef STARTUP_CHECKS_H
#define STARTUP_CHECKS_H

#include "integration/types.h"
#include "nanners_protocol/frame.h"

static_assert(sizeof(DrivePayload) <= kMaxPayloadSize, "DrivePayload too large!");
static_assert(sizeof(ToolPayload) <= kMaxPayloadSize, "ToolPayload too large!");
static_assert(sizeof(FunctionPayload) <= kMaxPayloadSize, "FunctionPayload too large!");
static_assert(sizeof(FeedbackPayload) <= kMaxPayloadSize, "FeedbackPayload too large!");

#endif //STARTUP_CHECKS_H
