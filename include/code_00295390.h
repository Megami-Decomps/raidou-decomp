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

s32 sbtlCallSetOt2(struct SBTLUNIT_WORK *wk, SBTLUNIT_MARK *marks, s32 count);

#endif
