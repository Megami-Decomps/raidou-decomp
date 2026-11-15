#ifndef SDFRAND_H
#define SDFRAND_H

#include "common.h"

typedef struct sdfRandCB_t {
    u32 v[4];
} sdfRandCB_t;

void sdfRandomize(sdfRandCB_t *aCB, u32 aSeed);
void sdfRandomizeByTime(sdfRandCB_t *aCB);

#endif
