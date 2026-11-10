/* Declarations of smt3-decomp src/smt3/sdf/sdfFile.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFFILE_H
#define SHARED_SDF_SDFFILE_H

#include "fpu.h"

#include "common.h"

#include "libsdf.h"

#include "ps2asm.h"

s32 strlen(const char *aStr);

s32 sdfCreateSema(s32 initCount, s32 maxCount, s32 option);

s32 sdfFileCmdClose(void *aHandle);

s32 sdfFileCmdFree(sdfFileCmd_t *aHandle);

void sdfChipFreeDirect(void *aPtr);

extern u32 sdfFileLengthWork;

extern u32 sdfFileSeekWork;

s32 sdfFileCmdFileSize(void *aHandle);

s32 sdfFileCmdSeek(void *aHandle, s32 aOffset, s32 aWhence);

extern s32 WaitSema(s32 aSemaId);

extern s32 SignalSema(s32 aSemaId);

extern s32 sdfFileSemaId;

extern u8 sdfFileDefaultSrcType;

extern u8 sdfFileCmdDefaultDevice;

s32 sdfFileCmdRead(void *aHandle, void *aBuffer, s32 aLength);

extern char sdfFileHostRootPath[];

s32 sceClose(s32 a);

char *strcat(char *aDest, const char *aSrc);

typedef struct sdfHostPrefix_t {
    s8 b[7];
} sdfHostPrefix_t;

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

typedef struct sdfDevicePrefix_t {
    s32 head;
    u16 tail;
} sdfDevicePrefix_t;

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

extern s32 DIntr();

extern void *memcpy(void *, const void *, size_t);

#include "sdk_libkernl.h"

extern char sdfFileCdromPrefix[];

extern char * strcpy(char *, const char *);

extern sdfHostPrefix_t sdfFileHostPrefix[];

extern u8 sdfFileHddPrefix[];

void *sdfChipAllocDirect(s32 aSize);

extern char * strchr(const char *, s32);

extern char sdfFileCdromName[];

extern s32 strncmp(const char *, const char *, size_t);

extern sdfFileDeviceCB_t sdfFileDeviceCB[];

extern void EIntr(void);

extern sdfFileCmdHandle_t *sdfFileCmdActiveHead;

extern sdfFileCmdHandle_t *sdfFileCmdFreeList;

extern s16 sdfFileCmdFreeCount;

sdfFileCmdHandle_t *sdfFileCmdReadOpen(const char *path, void (*callback)(sdfFileCmdHandle_t *, s32, saddr_t, s32, s32),
                        s32 callbackContext);

s32 sdfFileCmdClearError(sdfFileCmd_t *aHandle);

void sdfFileCmdStartThread(s32 deviceType);

extern s32 (*p2sdSmgBuf)(void);

s32 sdfCdImageCheckExist(const char *fname);

/* Convert a relative disc path to the drive's uppercase backslash form with ;1 suffix. */
void sdfFileSetCdFilename(char *aDst, char *aSrc);

char *sdfFileSetHostFilename(char *aDst, char *aSrc);

void sdfFileSetHddFilename(char *aDst, char *aSrc);

s32 sdfFileChkCdFile(char *aFilename);

s32 sdfFileChkHostFile(char *aFilename);

s32 sdfFileChkHddFile(char *aFilename);

/* Return an allocated device path, normalizing disc-path case and separators. */
char *sdfFileMakeFilename(char *aFilename);

s32 sdfFileCallback(void *aCmd, s32 aOp, s32 aArg, s32 aResult);

/* Open a path using the shared reply callback and wait for its initial completion. */
sdfFileCmdHandle_t *sdfFileOpen(const char *aFileName);

s32 sdfFileCheckExist(char *aFilename);

void sdfFileClose(void *aHandle);

s32 sdfFileRead(void *aHandle, void *aPtr, s32 aLeng);

u32 sdfFileLength(void *aHandle);

u32 sdfFileSeek(void *aHandle, s32 aPos, s32 aWhence);

/* Set the current device directory, ending in a slash; path is assumed nonempty. */
void sdfFileSetHostRoot(char *aRootPath);

char *sdfFileGetHostRoot(void);

void sdfFileCmdSetDefaultDevice(s32 aDevice);

void sdfFileSetDefaultSrc(s32 aType);

void sdfFileCmdSetDefaultDevice(s32 aDevice);

/* Queue a request on the global list and on its worker's list; wake the worker when its list was empty. */
void sdfFileCmdAllocHandle(sdfFileCmdHandle_t *state);

#endif
