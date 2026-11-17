#ifndef NWDNEWS_H
#define NWDNEWS_H

#include "common.h"
#include "kernel.h"

struct tagNWDNEWS_WORK {
    struct sdfMemHandle_t *hMem; /* 0x00 */
    dds3ProcessID_t *drawPID;    /* 0x04 */
    u8 unk08[8];
    u8 step;                     /* 0x10 */
    u8 flag;                     /* 0x11: bit 0 ends the process */
    s16 count;                   /* 0x12 */
    u8 unk14[8];
};

s32 Process_nwdNews(dds3ProcessID_t *PID);
s32 DrawProcess_nwdNews(dds3ProcessID_t *PID);

void Release_nwdNews(struct tagNWDNEWS_WORK *wk);
void Shutdown_nwdNews(dds3ProcessID_t *PID);
void nwdCallNext(void);
void nwdLoadSprite(void);
s32 nwdProcessCheck(void);
void nwdProcessEnd(void);
void nwdProcessStart(void);

#endif
