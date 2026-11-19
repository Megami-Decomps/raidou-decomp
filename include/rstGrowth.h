#ifndef RSTGROWTH_H
#define RSTGROWTH_H

#include "common.h"
#include "kernel.h"

/* Work of the item growth process (0x2C bytes). Partial layout. */
typedef struct tagRST_GROWTH_WORK {
    struct sdfMemHandle_t *hMem;
    u8 unk04[0x28];
} tagRST_GROWTH_WORK;

extern const char rstStrResultGrowth[];
s32 cmpLoadLocalizeText(void);
void rstInit(struct tagRST_GROWTH_WORK *wk);
s32 MenuCursorCtrl(dds3ProcessID_t *PID);
s32 rstCallGetProcessData42(dds3ProcessID_t *PID);

void rstCallProcEnd(struct tagRST_GROWTH_WORK *wk);
dds3ProcessID_t *rstCmpItemGrowthStart(void);
dds3ProcessID_t *rstGetGiftMsgPID(void);
void rstProcessEnd(void);
s32 rstSetFlagPlayBgm(void);

#endif
