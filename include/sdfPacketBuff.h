#ifndef SDFPACKETBUFF_H
#define SDFPACKETBUFF_H

#include "common.h"

s32 sdfGrAllocPacketBuff(s32 aSize);
void sdfGrMakePacketBuff(s32 aPacketBuffSize);
void sdfGrSetupPacketBuff(s32 aFrameId);

#endif
