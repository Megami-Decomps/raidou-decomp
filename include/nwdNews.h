#ifndef NWDNEWS_H
#define NWDNEWS_H

#include "common.h"
#include "kernel.h"

struct tagNWDNEWS_WORK {
    struct sdfMemHandle_t *hMem;
    dds3ProcessID_t *drawPID;
    u8 step;
    s32 flag;
    s16 count;
    f32 x;
    f32 y;
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
