#ifndef SDFGRAPH_H
#define SDFGRAPH_H

#include "common.h"

void sdfDeliverDrawEnd(s32 mode, s32 frameId, s32 halfOffset);
void sdfDeliverDrawStart(s32 frameId, s32 buffId);
void sdfGrInit(s32 aPacketBuffSize);
void sdfGrSetScreen(s32 aMode);
void sdfGrInitNewFrame(void);

#endif
