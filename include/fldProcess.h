#ifndef FLDPROCESS_H
#define FLDPROCESS_H

#include "common.h"

void fldEncountClearFlag(u32 flag);

struct SceneParam_t;

void fldProcTrigSequence(struct SceneParam_t *srcParam, s32 mode);
void mapCallMap(s32 map, s32 flor, s32 point, char **label);

#endif
