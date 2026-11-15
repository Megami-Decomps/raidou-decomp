#ifndef NWDNEWS_H
#define NWDNEWS_H

#include "common.h"
#include "kernel.h"

struct tagNWDNEWS_WORK;

void Release_nwdNews(struct tagNWDNEWS_WORK *wk);
void Shutdown_nwdNews(dds3ProcessID_t *PID);
void nwdCallNext(void);
void nwdLoadSprite(void);
s32 nwdProcessCheck(void);
void nwdProcessEnd(void);
void nwdProcessStart(void);

#endif
