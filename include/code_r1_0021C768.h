#ifndef CODE_R1_0021_C768_H
#define CODE_R1_0021_C768_H

#include "common.h"
#include "code_r1_0021CE60.h"

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

void *fileResolvePrimaryBuffer(FileJobPayload *job);

void *fileResolveSecondaryBuffer(FileJobPayload *job);

FileJobPayload *Create_effParts(FileJobPayload *request);

#endif
