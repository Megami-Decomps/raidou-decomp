/* Declarations of smt3-decomp src/smt3/sdf/sdfFileIO.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFFILEIO_H
#define SHARED_SDF_SDFFILEIO_H

#include "fpu.h"

#include "common.h"

#include "libsdf.h"

#include "ps2asm.h"

s32 strlen(const char *aStr);

s32 sdfCreateSema(s32 initCount, s32 maxCount, s32 option);

s32 sdfFileCmdClose(void *aHandle);

s32 sdfFileCmdFree(sdfFileCmd_t *aHandle);

s32 sdfFileCmdFileSize(void *aHandle);

s32 sdfFileCmdSeek(void *aHandle, s32 aOffset, s32 aWhence);

extern s32 WaitSema(s32 aSemaId);

extern s32 SignalSema(s32 aSemaId);

extern u8 sdfFileCmdThreadPrioCheckCounter;

void sdfFileCmdThreadSetPrio(s32 prio);

s32 sdfFileCmdRead(void *aHandle, void *aBuffer, s32 aLength);

void sdfFileCmdUnlink(void *aHandle);

void sdfFileCmdDequeue(void *aCmd);

s32 sceClose(s32 a);

typedef u8 sdfDevOperationCode_t;

typedef s8 sdfDevStateCode_t;

/* Complete 0x40-byte request owner. sdfFileCmd_t describes only its opaque
 * 0x30-byte prefix for consumers outside the device queue implementation. */
typedef struct sdfFileCmdHandle_t {
    struct sdfFileCmdHandle_t *next;
    struct sdfFileCmdHandle_t *prev;
    struct sdfFileCmdHandle_t *deviceNext;
    struct sdfFileCmdHandle_t *devicePrev;
    char *filename;
    u8 device;
    sdfDevOperationCode_t cmd;
    sdfDevStateCode_t stat;
    u8 phase;
    s32 offset;
    s32 length;
    void *buffer;
    s32 cmdParm;
    s32 fd;
    s32 errorCode;
    s32 currentOffset;
    u8 remainder[4];
    void (*callback)(struct sdfFileCmdHandle_t *, s32, saddr_t, s32, s32);
    s32 userData;
} sdfFileCmdHandle_t;

typedef struct sdfFileDeviceCB_t {
    s32 thId;
    s32 semaId;
    struct sdfFileCmdHandle_t *first;
    struct sdfFileCmdHandle_t *last;
    u8 opaque10[8];
} sdfFileDeviceCB_t;

enum SdfFileIOCmdStat {
    SDF_FILE_CMD_STAT_INIT = 0,
    SDF_FILE_CMD_STAT_DORMANT = 1,
    SDF_FILE_CMD_STAT_OPEN = 2,
    SDF_FILE_CMD_STAT_SEEK = 3,
    SDF_FILE_CMD_STAT_READ = 4,
    SDF_FILE_CMD_STAT_WRITE = 5,
    SDF_FILE_CMD_STAT_CLOSE = 6,
    SDF_FILE_CMD_STAT_SUSPEND = 7,
    SDF_FILE_CMD_STAT_DONE = 8,
    SDF_FILE_CMD_STAT_ERROR = 9,
    SDF_FILE_CMD_STAT_CANCEL = 10
};

enum SdfFileIOCmd {
    SDF_FILE_CMD_NONE = 0,
    SDF_FILE_CMD_READOPEN = 1,
    SDF_FILE_CMD_WRITEOPEN = 2,
    SDF_FILE_CMD_SEEK = 3,
    SDF_FILE_CMD_FILESIZE = 4,
    SDF_FILE_CMD_READ = 5,
    SDF_FILE_CMD_WRITE = 6,
    SDF_FILE_CMD_CLOSE = 7,
    SDF_FILE_CMD_READFILE = 8,
    SDF_FILE_CMD_WRITEFILE = 9
};

enum sdf_tex_resource {
    SDF_TEX_RESOURCE_FREE = 0,
    SDF_TEX_RESOURCE_FRAME = 1,
    SDF_TEX_RESOURCE_TEXTURE = 2,
    SDF_TEX_RESOURCE_CLUT = 3,
    SDF_TEX_RESOURCE_KNOWN_MODE_COUNT = 4
};

enum sdf_stream_output {
    SDF_STREAM_OUTPUT_RGBA32 = 0,
    SDF_STREAM_OUTPUT_RGBA16 = 1,
    SDF_STREAM_OUTPUT_RGBA16_DITHERED = 2
};

enum sdf_stream_playback {
    SDF_STREAM_PLAYBACK_INITIAL = 0,
    SDF_STREAM_PLAYBACK_FIRST_COMPLETION = 1,
    SDF_STREAM_PLAYBACK_CADENCED = 2
};

#include "sdk_libkernl.h"

/* Declared here because no header carries these. */
extern s32 sdfCdImageLoadStart(const char *filename);

extern void *memcpy(void *, const void *, size_t);

extern s32 sceLseek(s32 fileHandle, s32 offset, s32 origin);

extern s32 sceRead(s32 fileHandle, saddr_t buffer, s32 byteCount);

extern char * strcpy(char *, const char *);

void *sdfChipAllocDirect(s32 aSize);

extern sdfFileDeviceCB_t sdfFileDeviceCB[];

extern s32 DIntr(void);

extern void EIntr(void);

extern void sdfChipFreeDirect(void *aPtr);

extern sdfFileCmdHandle_t *sdfFileCmdActiveHead;

extern sdfFileCmdHandle_t *sdfFileCmdFreeList;

extern s16 sdfFileCmdCount;

extern sdfFileCmdHandle_t *sdfFileCmdCacheHead;

extern sdfFileCmdHandle_t *sdfFileCmdCacheTail;

extern s16 sdfFileCmdFreeCount;

u32 sdfMemLock(sdfMemHandle_t *aHandle);

extern char sdfFileCmdCdromPrefix[];

extern char sdfFileCmdHostPrefix[];

extern char sdfFileCmdPfsPrefix[];

extern char sdfFileCmdMoviePath[];

extern char sdfFileCmdCdimgPrefix[];

extern u8 sdfFileCmdDefaultDevice;

extern s32 memcmp(const void *aFirst, const void *aSecond, size_t aSize);

extern char *strchr(const char *aString, s32 aCharacter);

void *sdfChipAllocZeroDirect(s32 aSize);

extern s32 sdfFileCmdAnalyzeFileName(const char *aFileName, char **aRealNamePtr);

extern u16 sdfFileCmdDefaultCreateMode;

extern s32 sdfFileCmdThreadPrio;

extern s32 ChangeThreadPriority(s32 tid, s32 prio);

s32 sdfCreateThreadWithStack(void (*entryAddress)(), s32 stackBytes, s32 priority);

sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

void sdfCdImageClose(void);

sdfFileCmdHandle_t *sdfFileCmdReadOpen(const char *path, void (*callback)(sdfFileCmdHandle_t *, s32, saddr_t, s32, s32),
                        s32 callbackContext);

s32 sdfFileCmdClearError(sdfFileCmd_t *aHandle);

void sdfFileCmdStartThread(s32 deviceType);

extern s32 (*p2sdSmgBuf)(void);

s32 sdfCdImageGetCurrFileSize(void);

/* Consume up to size disc-file bytes, retaining a partial sector; return the byte count. */ s32 sdfCdImageRead(void *buffer, s32 byteCount);

/* Queue a request on the global list and on its worker's list; wake the worker when its list was empty. */ void sdfFileCmdAllocHandle(sdfFileCmdHandle_t *state);

char *sdfFileGetHostRoot(void);

/* Unlink a cached request under interrupt protection, then release its storage. */
void sdfFileCmdUnlink(void *aHandle);

/* Move a completed request off both active lists and into the bounded cache. */
void sdfFileCmdDequeue(void *aCmd);

void sdfFileCmdReleaseHandle(sdfFileCmd_t *aHandle);

/* Store an error/result, release the request, and notify its callback of inactivity. */
void sdfFileCmdFinish(sdfFileCmdHandle_t *state, s32 result);

void sdfFileCmdThread(sdfFileDeviceCB_t *aCB);

/* Worker slot 3 services disc-backed requests through serialized disc operations. */
void sdfFileCdImageCmdThread(sdfFileDeviceCB_t *aCB);

/* Allocate a worker-specific device path; an unsupported worker returns NULL. */
char *sdfFileCmdMakeRealFileName(const char *aSrcName, s32 aDeviceType);

/* DDS engine donor: dds-decomp (redthing1), sdfFileCmdAnalyzeFileName.
 * SMT3 has the /movie/ exception, but not DDS's extra /movie2/ prefix. */
s32 sdfFileCmdAnalyzeFileName(const char *aFileName, char **aRealNamePtr);

/* Allocate a request state and retain the callback's opaque context word. */
sdfFileCmdHandle_t *sdfFileCmdNewHandle(void *resource, s32 workerIndex, s32 operation,
                        void (*callback)(sdfFileCmdHandle_t *, s32, saddr_t, s32, s32), s32 callbackContext);

/* Resolve a path to its worker and enqueue an asynchronous read-only open. */
sdfFileCmdHandle_t *sdfFileCmdReadOpen(const char *path, void (*callback)(sdfFileCmdHandle_t *, s32, saddr_t, s32, s32),
                        s32 callbackContext);

/* Enqueue a write open; zero options select the device's default creation options. */
sdfFileCmdHandle_t *sdfFileCmdWriteOpen(const char *path, void (*callback)(sdfFileCmdHandle_t *, s32, saddr_t, s32, s32),
                        s32 callbackContext, s32 options);

/* Queue a seek with its offset and origin; return -1 unless the request is active. */
s32 sdfFileCmdSeek(void *aHandle, s32 aOffset, s32 aWhence);

/* Query file size without changing the file position; return -1 unless active. */
s32 sdfFileCmdFileSize(void *aHandle);

/* Queue a byte-counted read; return -1 without changing an inactive request. */
s32 sdfFileCmdRead(void *aHandle, void *aBuffer, s32 aLength);

/* Queue a byte-counted write; return -1 without changing an inactive request. */
s32 sdfFileCmdWrite(sdfFileCmdHandle_t *aHandle, const void *aBuffer, s32 aLength);

s32 sdfFileCmdClearError(sdfFileCmd_t *aHandle);

/* Active state and close operation share an ID; retain the original byte assignment. */
s32 sdfFileCmdClose(void *aHandle);

s32 sdfFileCmdFree(sdfFileCmd_t *aHandle);

/* Enqueue a one-shot read: negative byte count requests the whole file. */
sdfFileCmdHandle_t *sdfFileCmdReadFile(const char *path, s32 buffer, s32 byteCount,
                        void (*callback)(sdfFileCmdHandle_t *, s32, saddr_t, s32, s32), s32 callbackContext);

/* Enqueue a one-shot write using the supplied buffer, byte count and creation options. */
sdfFileCmdHandle_t *sdfFileCmdWriteFile(const char *path, s32 buffer, s32 byteCount,
                        void (*callback)(sdfFileCmdHandle_t *, s32, saddr_t, s32, s32),
                        s32 callbackContext, s32 options);

/* Update non-negative worker IDs only; skip an unchanged numeric priority. */
void sdfFileCmdThreadSetPrio(s32 prio);

void sdfFileCmdThreadSetLoPrio(void);

void sdfFileCmdThreadSetHiPrio(void);

void sdfFileCmdThreadCheckPrio(void);

/* Start worker thread `index` if it isn't running; slot 3 runs the alternate entry point. */
void sdfFileCmdStartThread(s32 deviceType);

#endif
