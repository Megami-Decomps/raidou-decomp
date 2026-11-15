#ifndef SDFDELAYEDRESRC_H
#define SDFDELAYEDRESRC_H

#include "common.h"
#include "libsdf.h"

u64 sdfDelayedResrcCheck(void);
void sdfDelayedResrcInitCB(sdfDelayedResrcCB_t *aCB, sdfDelayedResrcFreeFunc aFreeFunc);
void sdfDelayedResrcEntry(sdfDelayedResrcCB_t *aCB, void *aItem);
void sdfDelayedResrcProc(void);

#endif
