#ifndef SDK_LIBPAD_H
#define SDK_LIBPAD_H

#include "common.h"

int scePadGetReqState(int port, int slot);
int scePadGetState(int port, int slot);
int scePadInfoAct(int port, int slot, int actno, int term);
int scePadInit(int mode);
s32 scePadPortOpen(s32 port, s32 slot, void *buffer);
int scePadRead(int port, int slot, u8 *data);
int scePadSetActAlign(int port, int slot, const u8 *align);
int scePadSetActDirect(int port, int slot, const u8 *data);
int scePadSetMainMode(int port, int slot, int offs, int lock);

#endif
