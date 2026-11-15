#ifndef MCMANAGER_H
#define MCMANAGER_H

#include "common.h"

/* One poll attempt's result. PENDING means no terminal result yet; the SDK
 * poll values 0 and -1 both map to it. */
typedef enum McPollResult {
    MC_POLL_PENDING = 0,
    MC_POLL_SUCCESS = 1,
    MC_POLL_ERROR = -1,
    MC_POLL_SPECIAL_ERROR = -2
} McPollResult;

void sdfMcRegetInfo(s32 port);
McPollResult sdfMcFuncGetInfo1(void);
s32 sdfMcGetFileInfoReady(s32 port);
s32 sdfMcGetFreeBytes(s32 port);
s32 sdfMcGetStatus(s32 port);
void sdfMcClearCardStat(s32 port);
void sdfMcSetCardStat(s32 port);
void sdfMcClearSlotFlags(s32 port, s32 slot);
void sdfMcSetSlotFlags(s32 port, s32 slot, u32 flags);
u32 sdfMcGetSlotFlags(s32 port, s32 slot);
s32 sdfMcGetSlotIndex(s32 port);
void sdfMcSetSlotIndex(s32 port, s32 slot);
void sdfMcFuncFormat(u32 port);
McPollResult sdfMcFuncFormat1(void);
void sdfMcFuncChdir(u32 port, const char *path);
McPollResult sdfMcFuncChdir1(void);
void sdfMcFuncMkdir(u32 mccb, const char *cb);
McPollResult sdfMcFuncMkdir1(void);
void sdfMcFuncGetDir(u32 port, const char *path, void *directoryEntries, s32 entryLimit);
McPollResult sdfMcFuncGetDir1(s32 *commandResultOut);
void sdfMcFuncDelete(u32 port, const char *path);
McPollResult sdfMcFuncDelete1(void);
void sdfMcFuncOpen(u32 port, const char *path, s32 openFlags);
McPollResult sdfMcFuncOpen1(s32 *commandResultOut);
void sdfMcFuncClose(s32 descriptor);
McPollResult sdfMcFuncClose1(void);
void sdfMcFuncRead(s32 descriptor, void *buffer, s32 size);
McPollResult sdfMcFuncRead1(void);
void sdfMcFuncWrite(s32 descriptor, const void *buffer, s32 size);
McPollResult sdfMcFuncWrite1(void);

#endif
