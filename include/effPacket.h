#ifndef EFFPACKET_H
#define EFFPACKET_H

#include "common.h"
#include "libsdf.h"

void *effPkt3DPrimMakePoly(sdf3DPrimPolyParm_t *aParm);

void *effPkt2DGetPrimPtr(sdfGrPacketHeader_t *pPkt);

sdfGrPacketHeader_t *effPkt2DAllocTriStrip(s32 num, s32 prim);

u64 *effPkt2DAllocF4(u32 prim);

/* sdfGrOtTable entries the crossfade files its capture and its fading copy into. */
extern sdfGrOt_t dds3UpdateCrossfadeData;
extern sdfGrOt_t dds3Data9;
extern sdfGrOt_t dds3UpdateCrossfadeData2;
extern s8 dds3CrossfadeRunFlag;
void dds3CrossfadeEnd(void);

#endif
