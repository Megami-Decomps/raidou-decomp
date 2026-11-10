#include "common.h"

/* Declarations from smt3-decomp src/smt3/mc/mcManager.c */
#include "common.h"

#define MC_CARD_SLOT 0

#define MC_SYNC_POLL_MODE 1

#define MC_SYNC_COMPLETE 1

#define MC_SDK_COMMAND_SUCCESS 0

#define MC_SDK_SPECIAL_ERROR (-4)

#define MC_SDK_EXTENDED_ERROR_LIMIT (-10)

/* One poll attempt's result. PENDING means no terminal result yet; the SDK
 * poll values 0 and -1 both map to it. */
typedef enum McPollResult {
    MC_POLL_PENDING = 0,
    MC_POLL_SUCCESS = 1,
    MC_POLL_ERROR = -1,
    MC_POLL_SPECIAL_ERROR = -2
} McPollResult;

extern s32 sceMcSync(s32 a, void *b, s32 *c);

extern s32 sceMcGetInfo(s32 a, s32 b, s32 *c, s32 *d, s32 *e);

extern s32 sceMcChdir(s32 a, s32 b, const char *c, char *d);

extern s32 sceMcGetDir(s32 a, s32 b, const char *c, u32 d, s32 e, void *f);

extern s32 sceMcOpen(s32 a, s32 b, const char *c, s32 d);

extern s32 sceMcFormat(u32 port, s32 slot);

extern s32 sceMcMkdir(u32 a, s32 b, const char *c);

extern s32 sceMcDelete(u32 port, s32 slot, const char *path);

extern s32 sceMcClose(s32 a);

extern s32 sceMcRead(s32 a, void *b, s32 c);

extern s32 sceMcWrite(s32 a, const void *b, s32 c);

/* One control block per port. */
typedef struct
{
    s32 syncResult;
    s32 type;
    s32 free;
    s32 format;
    u8 retry;
    u8 fileInfoReady;
    u8 cardStat;
    s8 slotIndex;
    u32 slotFlags[5];
} sdfMcCb_tag;

extern sdfMcCb_tag sdfMcCb[];

extern s32 sdfMcInfo;

void sdfMcFuncGetInfo(s32 port);

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
/* End of declarations from src/smt3/mc/mcManager.c */

INCLUDE_ASM(const s32, "mc/mcManager", func_001EAE30);

INCLUDE_ASM(const s32, "mc/mcManager", func_001EAE78);

INCLUDE_ASM(const s32, "mc/mcManager", func_001EAF70);

INCLUDE_ASM(const s32, "mc/mcManager", func_001EAF98);

INCLUDE_ASM(const s32, "mc/mcManager", func_001EAFC0);

INCLUDE_ASM(const s32, "mc/mcManager", func_001EAFE8);

INCLUDE_ASM(const s32, "mc/mcManager", func_001EB010);

INCLUDE_ASM(const s32, "mc/mcManager", func_001EB040);

INCLUDE_ASM(const s32, "mc/mcManager", func_001EB070);

INCLUDE_ASM(const s32, "mc/mcManager", func_001EB0A8);

INCLUDE_ASM(const s32, "mc/mcManager", func_001EB0D8);

INCLUDE_ASM(const s32, "mc/mcManager", func_001EB100);

void sdfMcFuncFormat(u32 port)
{
    sceMcFormat(port, MC_CARD_SLOT);
}

McPollResult sdfMcFuncFormat1(void)
{
    u32 commandId;
    s32 commandResult;
    s32 pollResult = sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult);

    if (pollResult != MC_SYNC_COMPLETE) {
        return MC_POLL_PENDING;
    }
    if (commandResult == MC_SDK_COMMAND_SUCCESS) {
        return pollResult;
    }
    return MC_POLL_ERROR;
}

void sdfMcFuncChdir(u32 port, const char *path)
{
    sceMcChdir(port, MC_CARD_SLOT, path, 0);
}

McPollResult sdfMcFuncChdir1(void)
{
    s32 commandId;
    s32 commandResult;

    if (sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult) == MC_SYNC_COMPLETE) {
        if (commandResult == MC_SDK_COMMAND_SUCCESS) {
            return MC_POLL_SUCCESS;
        }
        return commandResult == MC_SDK_SPECIAL_ERROR ? MC_POLL_SPECIAL_ERROR : MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}

void sdfMcFuncMkdir(u32 mccb, const char *cb)
{
    sceMcMkdir(mccb, MC_CARD_SLOT, cb);
}

McPollResult sdfMcFuncMkdir1(void)
{
    u32 commandId;
    s32 commandResult;
    s32 pollResult = sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult);

    if (pollResult == MC_SYNC_COMPLETE) {
        if (commandResult == MC_SDK_COMMAND_SUCCESS) {
            return pollResult;
        }
        if (commandResult < MC_SDK_EXTENDED_ERROR_LIMIT) {
            return MC_POLL_SPECIAL_ERROR;
        }
        return MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}

void sdfMcFuncGetDir(u32 port, const char *path, void *directoryEntries, s32 entryLimit)
{
    sceMcGetDir(port, MC_CARD_SLOT, path, 0, entryLimit, directoryEntries);
}

McPollResult sdfMcFuncGetDir1(s32 *commandResultOut)
{
    u32 commandId;
    s32 commandResult;
    s32 pollResult = sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult);

    if (pollResult == MC_SYNC_COMPLETE) {
        if (commandResult >= MC_SDK_COMMAND_SUCCESS) {
            *commandResultOut = commandResult;
            return pollResult;
        }
        if (commandResult == MC_SDK_SPECIAL_ERROR) {
            return MC_POLL_SPECIAL_ERROR;
        }
        return MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}

void sdfMcFuncDelete(u32 port, const char *path)
{
    sceMcDelete(port, MC_CARD_SLOT, path);
}

McPollResult sdfMcFuncDelete1(void)
{
    s32 commandId;
    s32 commandResult;

    if (sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult) == MC_SYNC_COMPLETE) {
        if (commandResult == MC_SDK_COMMAND_SUCCESS) {
            return MC_POLL_SUCCESS;
        }
        return commandResult == MC_SDK_SPECIAL_ERROR ? MC_POLL_SPECIAL_ERROR : MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}

void sdfMcFuncOpen(u32 port, const char *path, s32 openFlags)
{
    sceMcOpen(port, MC_CARD_SLOT, path, openFlags);
}

McPollResult sdfMcFuncOpen1(s32 *commandResultOut)
{
    u32 commandId;
    s32 commandResult;
    s32 pollResult = sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult);

    if (pollResult == MC_SYNC_COMPLETE) {
        if (commandResult >= MC_SDK_COMMAND_SUCCESS) {
            *commandResultOut = commandResult;
            return pollResult;
        }
        if (commandResult == MC_SDK_SPECIAL_ERROR) {
            return MC_POLL_SPECIAL_ERROR;
        }
        return MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}

void sdfMcFuncClose(s32 descriptor)
{
    sceMcClose(descriptor);
}

McPollResult sdfMcFuncClose1(void)
{
    s32 commandId;
    s32 commandResult;

    if (sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult) == MC_SYNC_COMPLETE) {
        if (commandResult == MC_SDK_COMMAND_SUCCESS) {
            return MC_POLL_SUCCESS;
        }
        return commandResult == MC_SDK_SPECIAL_ERROR ? MC_POLL_SPECIAL_ERROR : MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}

void sdfMcFuncRead(s32 descriptor, void *buffer, s32 size)
{
    sceMcRead(descriptor, buffer, size);
}

McPollResult sdfMcFuncRead1(void)
{
    u32 commandId;
    s32 commandResult;
    s32 pollResult = sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult);

    if (pollResult == MC_SYNC_COMPLETE) {
        if (commandResult >= MC_SDK_COMMAND_SUCCESS) {
            return pollResult;
        }
        if (commandResult == MC_SDK_SPECIAL_ERROR) {
            return MC_POLL_SPECIAL_ERROR;
        }
        return MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}

void sdfMcFuncWrite(s32 descriptor, const void *buffer, s32 size)
{
    sceMcWrite(descriptor, buffer, size);
}

McPollResult sdfMcFuncWrite1(void)
{
    s32 commandId;
    s32 commandResult;
    s32 pollResult = sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult);

    if (pollResult == MC_SYNC_COMPLETE) {
        if (commandResult >= MC_SDK_COMMAND_SUCCESS) {
            return pollResult;
        }
        if (commandResult == MC_SDK_SPECIAL_ERROR) {
            return MC_POLL_SPECIAL_ERROR;
        }
        return MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}
