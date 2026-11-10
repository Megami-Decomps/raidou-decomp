/* Declarations of smt3-decomp src/smt3/sdf/sdfCd.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFCD_H
#define SHARED_SDF_SDFCD_H

#include "fpu.h"

#include "common.h"

#include "libsdf.h"

#include "ps2asm.h"

typedef struct
{
    u32 nameOffset_;
    u32 offset_;
    s32 size_;
} sdfCdImageDirEntry_t;

s32 sdfCreateSema(s32 initCount, s32 maxCount, s32 option);

s32 sceCdInit(s32 aMode);

s32 sceCdMmode(s32 aMode);

s32 sceCdStStop(void);

extern sdfCdImageDirEntry_t *sdfCdImageCurrFile;

extern s32 sdfCdImageFd;

extern s32 sdfCdTrayMode;

sdfCdImageDirEntry_t *sdfCdImageSearchFile(const char *fname, s32 *lsn, u8 *modePtr);

extern s32 WaitSema(s32 aSemaId);

extern s32 SignalSema(s32 aSemaId);

extern void *sdfCdImageDirData;

void FatalError(const char *aMessage, ...) __attribute__((noreturn));

s32 sceClose(s32 a);

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

extern void *memcpy(void *, const void *, size_t);

extern void sdfDelayThread(s32 delay);

void sdfCdWaitTrayClose(void);

extern u8 sdfCdImageMode;

extern s32 sdfCdCurrLsn;

extern s32 sceCdStStart(s32 arg, u8 *options);

extern s32 sceCdStatus(void);

extern s32 sdfCdImageCurrRemain;

extern s32 sdfCdImageCurrActive;

extern u8 *sdfCdImageOffset;

extern u8 sdfCdSectorBuf[0x800];

#include "sdk_libkernl.h"

extern s32 sceLseek(s32 fileHandle, s32 offset, s32 origin);

extern void *malloc(size_t size);

extern s32 sceRead(s32 fileHandle, saddr_t buffer, s32 byteCount);

extern const char *dds3ImgPath;

#include "sdk_libcdvd.h"

extern u32 sdfCdImageFileName;

extern u32 dds3ImgLsn;

extern s32 sceSifAllocIopHeap(s32);

extern void sceCdStInit(s32, s32, s32);

void sdfCdImageClose(void);

extern s32 (*p2sdSmgBuf)(void);

void *sdfCdImageGetDirData(void);

#include "sdk_libc.h"

extern s32 sceCdDiskReady(s32 mode);

extern s32 sceCdGetDiskType(void);

extern char sdfCdMsgWaitTrayClose[];

extern char sdfCdMsgTrayClose[];

extern char sdfCdMsgReadRestart[];

void sdfCdWaitTrayClose(void);

void sdfCdStStart(s32 lsn);

void sdfCdStRead(s32 sectors, void *buffer);

extern char *strcpy(char *destination, const char *source);

extern char *strchr(const char *text, s32 character);

extern sdfCdImageDirEntry_t sdfCdImageSearchFileDirect_ent;

sdfCdImageDirEntry_t *sdfCdImageSearchFileDirect(const char *filename, s32 *lsn, u8 *mode);

extern char sdfCdDirectPrefix[];

extern s32 memcmp(const void *left, const void *right, size_t size);

extern s32 strcmp(const char *left, const char *right);

sdfCdImageDirEntry_t *sdfCdImageSearchFile(const char *path, s32 *lsn, u8 *mode);

s32 sdfCdImageLoadStart(const char *filename);

/* Consume up to size disc-file bytes, retaining a partial sector; return the byte count. */
s32 sdfCdImageRead(void *buff, s32 length);

void sdfCdImageClose(void);

s32 sdfCdImageCheckExist(const char *fname);

s32 sdfCdImageGetFileSize(const char *fname);

s32 sdfCdImageGetCurrFileSize(void);

/* Read a whole named file into the loader's allocated EE buffer, then close it. */
void sdfCdImageLoadDir(const char *dirFile);

/* Record the disc search path, preload a separate file, and initialize an aligned IOP buffer. */
void sdfCdImageInit(const char *imageFile, const char *dirFile);

s32 sdfCdInit(void);

#endif
