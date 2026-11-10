/* Declarations of smt3-decomp src/smt3/file/fileManager.c, for its bodies in this directory. */
#ifndef SHARED_FILE_FILEMANAGER_H
#define SHARED_FILE_FILEMANAGER_H

#include "common.h"

#include "libsdf.h"

#include "kernel.h"

#include "fileManager.h"

/* All three views share the 0x30-byte request header. Their differently
 * typed links distinguish transfer work from the queue and PAC callbacks. */
typedef void (*fileTransferCallback_t)(void *request, u32 arg);

#define FILE_REQUEST_FIELDS(Link, Callback) \
    u8 kind; \
    u8 state; \
    u8 retry; \
    u8 mode; \
    Link *next; \
    char *name; \
    void *dev; \
    s32 rest; \
    s32 size; \
    Callback callback; \
    void *arg; \
    sdfMemHandle_t *handle; \
    addr_t top; \
    addr_t ptr; \
    Link *donenext; \
    sdfPacDecodeCb_t pac; \
    u16 pacrest; \
    u16 slot; \
    Link *cbnext;

typedef struct fileTransferJob {
    FILE_REQUEST_FIELDS(struct fileTransferJob, fileTransferCallback_t)
} fileCb_t;

typedef struct pacFileRequest {
    FILE_REQUEST_FIELDS(struct pacFileRequest, void *)
} pacFileRequest_t;

#undef FILE_REQUEST_FIELDS

typedef struct fileSlot {
    /* 0x0 */ u32 size;
    /* 0x4 */ fileRequest_t *req;
} fileSlot_t;

typedef struct fileQueue {
    /* 0x00 */ s32 semaId;
    /* 0x04 */ u8 index;
    /* 0x05 */ u8 readIndex;
    /* 0x06 */ u8 activeCount;
    /* 0x07 */ u8 freeCount;
    /* 0x08 */ fileRequest_t *queue;
    /* 0x0C */ fileRequest_t *queueTail;
    /* 0x10 */ fileCb_t *endCallbackLink;
    /* 0x14 */ pacFileRequest_t *pacEndCallbackLink;
    /* 0x18 */ fileRequest_t *closeLink;
    /* 0x1C */ u8 *buffer;
    /* 0x20 */ fileSlot_t data[4];
} fileMngCb_tag;

extern fileMngCb_tag fileQueueHead;

extern u32 fileCallbackStatus;

extern s32 (*p2sdSmgBuf)(void);

extern char fileManagerProcName[];

void fileLinkQueue(fileRequest_t *req, s32 kind, const char *name, void *callback, void *arg);

void fileLinkCloseWait(fileMngCb_tag *cb, fileRequest_t *fileHandle);

void fileUnlinkCloseWait(fileMngCb_tag *cb, fileRequest_t *fileHandle);

fileRequest_t *pacLoadEx(const char *filename, u32 headOrTail, u32 callback, u32 pendCallback, u32 callbackParm);

s32 pacFileCheckEnd(const fileRequest_t *pacFileHandle);

void pacRead(fileRequest_t *pacHandle);

s32 pacLoadCallback(void *fileHandle, s32 cause, s32 buffer, s32 length, fileCb_t *data);

s32 fileLoadCallback(void *fileHandle, s32 cause, s32 buffer, s32 length, fileCb_t *data);

s32 fileSaveCallback(void *fileHandle, s32 cause, s32 buffer, s32 length, fileCb_t *data);

void fileRead(fileCb_t *fileHandle);

void fileExecQueue(void);

s32 fileManager(void);

s32 fileManagerProc(dds3ProcessID_t *procId);

dds3ProcessID_t *dds3StartProcess(char *v1, s32 dDS3_PRIORITY_DRAW_PB, s32 v2, s32 v3,
                                s32 (*drawExit)(dds3ProcessID_t *),
                                void (*deleteExit)(dds3ProcessID_t *), void *p);

s32 sdfCreateSema(s32 initCount, s32 maxCount, s32 option);

s32 WaitSema(s32 sema);

s32 SignalSema(s32 sema);

void *sdfChipAllocZeroDirect(u32 aSize);

char *sdfStrdupChip(const char *str);

void *sdfMemAllocDirect(u32 aSize);

void *sdfMemAlloc2Direct(u32 aSize);

u32 sdfMemLock(void *aHandle);

void sdfPacInit(void *cb, void *callback);

void sdfPacSetFromTail(void *cb);

void *sdfFileCmdReadOpen(const char *aFileName, void *aCallback, void *aUserData);

void *sdfFileCmdWriteOpen(const char *aFileName, void *aCallback, void *aUserData, u32 aMode);

s32 sdfFileCmdFileSize(void *aHandle);

s32 sdfFileCmdRead(void *aHandle, void *aBuffer, s32 aLength);

s32 sdfFileCmdWrite(void *aHandle, void *aBuffer, s32 aLength);

s32 sdfFileCmdClose(void *aHandle);

s32 sdfFileCmdFree(void *aHandle);

void sdfFileCmdThreadSetHiPrio(void);

s32 sdfPrintf(const char *str, ...);

void *memset(void *a, s32 b, size_t c);

void sdfChipFreeDirect(void *aPtr);

void sdfPacFree(sdfPacDecodeCb_t *cb);

s32 sdfPacDecode(sdfPacDecodeCb_t *cb, const u8 *data, s32 length);

void *sdfMemAllocFromTailDirect(u32 aSize);

void fileWrite(fileCb_t *fileHandle);

void fileExecCallback(void);

s32 pacFileHandleFree(fileRequest_t *pacHandle);

s32 pacFileHandleFree_(void *pacHandle);

s32 fileHandleFree(fileRequest_t *fileHandle);

void fileLinkQueue(fileRequest_t *req, s32 kind, const char *name, void *callback, void *arg);

void fileUnlinkQueue(fileRequest_t *fileHandle);

void fileLinkCloseWait(fileMngCb_tag *cb, fileRequest_t *fileHandle);

void fileUnlinkCloseWait(fileMngCb_tag *cb, fileRequest_t *fileHandle);

fileRequest_t *pacLoadEx(const char *filename, u32 headOrTail, u32 callback, u32 pendCallback, u32 callbackParm);

fileRequest_t *pacLoad(const char *filename);

fileRequest_t *pacLoadUseTail(const char *filename);

fileRequest_t *fileLoadEx(const char *filename, u32 headOrTail, addr_t endCallback, addr_t callbackParm);

fileRequest_t *fileLoad(const char *filename);

fileRequest_t *fileLoadUseTail(const char *filename);

sdfMemHandle_t *fileGetMemHandle(const fileRequest_t *fileHandle);

u8 *fileGetLoadPtr(const fileRequest_t *fileHandle);

u32 fileGetLoadSize(const fileRequest_t *fileHandle);

u32 fileGetLoadRemain(fileRequest_t *fileHandle);

s32 pacFileCheckEnd(const fileRequest_t *pacFileHandle);

s32 pacCheckEnd(const fileRequest_t *pacHandle);

void fileSync(fileRequest_t *fileHandle);

void pacSync(fileRequest_t *pacFileHandle);

void fileSyncAll(void);

fileRequest_t *fileSaveEx(const char *filename, const void *buff, s32 size, s32 endCallback, s32 callbackParm);

fileRequest_t *fileSave(const char *filename, const void *buff, s32 size);

void pacRead(fileRequest_t *pacHandle);

s32 pacLoadCallback(void *fileHandle, s32 cause, s32 buffer, s32 length, fileCb_t *data);

void fileRead(fileCb_t *fileHandle);

s32 fileLoadCallback(void *fileHandle, s32 cause, s32 buffer, s32 length, fileCb_t *data);

void fileWrite(fileCb_t *fileHandle);

s32 fileSaveCallback(void *fileHandle, s32 cause, s32 buffer, s32 length, fileCb_t *data);

void fileExecQueue(void);

void fileExecCallback(void);

s32 fileManager(void);

s32 fileManagerProc(dds3ProcessID_t *procId);

void fileSysInit(void);

#endif
