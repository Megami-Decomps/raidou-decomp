#ifndef CODE_00295390_H
#define CODE_00295390_H

#include "common.h"
#include "enums.h"

struct SBTLUNIT_WORK;

/* Battle event handlers, indexed by SBTL_WORK.eventno while flg bit 0x400 is set. */
extern s32 (*sbtlRefSbtl29Data[])(void);
/* Helper-mark lists and their lengths for sbtlCallSetOt2, indexed by SBTL_MOTION_VISUAL.field2E. */
extern s32 sbtlCallBtlGetMotionVisualData3Data[];
extern SBTLUNIT_MARK *sbtlCallBtlGetMotionVisualData3Data2[];
/* Stage row index sbtlCallBitCheck last ran. */
extern s32 sbtlCallBitCheckData;
/* MSG1 blobs of the tutorial battle events, picked by sbtlGetEventMes from SBTL_WORK.eventno. */
extern u8 sbtlGetEventMesData[];
extern u8 sbtlGetEventMesData2[];
extern u8 sbtlGetEventMesData3[];
extern u8 sbtlGetEventMesData4[];
#ifndef VERSION_RAIDOU2
/* MSG1 blobs of the negotiation characters, picked by sbtlEvent_SetCharacterProc. */
extern u8 sbtlEvent_SetCharacterProcData[];
extern u8 sbtlEvent_SetCharacterProcData2[];
extern u8 sbtlEvent_SetCharacterProcData3[];
#endif

u8 *sbtlGetEventMes(s16 eventno);
s32 sbtlEvent_NegoProc(u8 *adr, s32 no);
s32 sbtlMessageProc(u8 *adr, s32 no, struct SBTLUNIT_WORK *wk, s32 flg);
s32 sbtlCallSetOt2(struct SBTLUNIT_WORK *wk, SBTLUNIT_MARK *marks, s32 count);

#endif
