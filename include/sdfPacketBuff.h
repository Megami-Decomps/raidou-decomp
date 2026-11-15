#ifndef SDFPACKETBUFF_H
#define SDFPACKETBUFF_H

#include "common.h"

s32 sdfGrAllocPacketBuff(s32 aSize);
void sdfGrMakePacketBuff(s32 aPacketBuffSize);
void sdfGrSetupPacketBuff(s32 aFrameId);
s32 sdfGrOpenPacketBuff(void);
void sdfGrClosePacketBuff(saddr_t aBuffPtr);

#endif
