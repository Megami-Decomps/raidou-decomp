#include "common.h"

/* Declarations from smt3-decomp src/smt3/sdf/sdfCd.c */
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

extern const char sdfCdMsgWaitTrayClose[];

extern const char sdfCdMsgTrayClose[];

extern const char sdfCdMsgReadRestart[];

void sdfCdWaitTrayClose(void);

void sdfCdStStart(s32 lsn);

extern const char sdfCdMsgWaitTrayClose[18];

extern const char sdfCdMsgTrayClose[16];

extern const char sdfCdMsgReadRestart[18];

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

extern char sdfCdDirectPrefix[8];

extern s32 sdfCdTrayMode;
/* End of declarations from src/smt3/sdf/sdfCd.c */

/* Declarations from dds-decomp src/dds1/sdf/sdfCd.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#define SMT3_DEFINES_SDF_FILE_CMD_CLOSE 1

#define SMT3_DEFINES_SDF_FILE_CMD_FILESIZE 1

#define SMT3_DEFINES_SDF_FILE_CMD_NONE 1

#define SMT3_DEFINES_SDF_FILE_CMD_READ 1

#define SMT3_DEFINES_SDF_FILE_CMD_READFILE 1

#define SMT3_DEFINES_SDF_FILE_CMD_READOPEN 1

#define SMT3_DEFINES_SDF_FILE_CMD_SEEK 1

#define SMT3_DEFINES_SDF_FILE_CMD_STAT_CANCEL 1

#define SMT3_DEFINES_SDF_FILE_CMD_STAT_CLOSE 1

#define SMT3_DEFINES_SDF_FILE_CMD_STAT_DONE 1

#define SMT3_DEFINES_SDF_FILE_CMD_STAT_DORMANT 1

#define SMT3_DEFINES_SDF_FILE_CMD_STAT_ERROR 1

#define SMT3_DEFINES_SDF_FILE_CMD_STAT_INIT 1

#define SMT3_DEFINES_SDF_FILE_CMD_STAT_OPEN 1

#define SMT3_DEFINES_SDF_FILE_CMD_STAT_READ 1

#define SMT3_DEFINES_SDF_FILE_CMD_STAT_SEEK 1

#define SMT3_DEFINES_SDF_FILE_CMD_STAT_SUSPEND 1

#define SMT3_DEFINES_SDF_FILE_CMD_STAT_WRITE 1

#define SMT3_DEFINES_SDF_FILE_CMD_WRITE 1

#define SMT3_DEFINES_SDF_FILE_CMD_WRITEFILE 1

#define SMT3_DEFINES_SDF_FILE_CMD_WRITEOPEN 1

#define SMT3_DEFINES_SDF_STREAM_OUTPUT_RGBA16 1

#define SMT3_DEFINES_SDF_STREAM_OUTPUT_RGBA16_DITHERED 1

#define SMT3_DEFINES_SDF_STREAM_OUTPUT_RGBA32 1

#define SMT3_DEFINES_SDF_STREAM_PLAYBACK_CADENCED 1

#define SMT3_DEFINES_SDF_STREAM_PLAYBACK_FIRST_COMPLETION 1

#define SMT3_DEFINES_SDF_STREAM_PLAYBACK_INITIAL 1

#define SMT3_DEFINES_SDF_TEX_RESOURCE_CLUT 1

#define SMT3_DEFINES_SDF_TEX_RESOURCE_FRAME 1

#define SMT3_DEFINES_SDF_TEX_RESOURCE_FREE 1

#define SMT3_DEFINES_SDF_TEX_RESOURCE_KNOWN_MODE_COUNT 1

#define SMT3_DEFINES_SDF_TEX_RESOURCE_TEXTURE 1

#define SMT3_DEFINES_SdfFileIOCmd 1

#define SMT3_DEFINES_SdfFileIOCmdStat 1

#define SMT3_DEFINES_sdfCdImageDirEntry_t 1

#define SMT3_DEFINES_sdf_stream_output 1

#define SMT3_DEFINES_sdf_stream_playback 1

#define SMT3_DEFINES_sdf_tex_resource 1

#include "sdk_libcdvd.h"

#include "dds/sdk_libkernl.h"

void FlushCache(s32 mode);

#include "dds/fpu.h"

#include "dds/common.h"

#include "dds/libsdf.h"

#include "dds/ps2asm.h"

#include "dds/sdk_libkernl.h"

#include "dds/sdk_libcdvd.h"

#include "dds/sdk_libc.h"

extern s32 sdfDiscRequestPending;

extern s32 sdfDiscRequestSemaphore;

void sdfDevSignalPendingSemaphore(void);

void sdfDevSeekDiscWithRequestGate(s32 lsn);
/* End of declarations from src/dds1/sdf/sdfCd.c */

INCLUDE_ASM(const s32, "sdf/sdfCd", func_003E19D0);

void sdfCdStStart(s32 lsn) {
    u8 options[4];
    s32 ready;
    options[0] = 0;
    if (sdfCdImageMode == 1) {
        options[1] = 0;
    } else {
        options[1] = 1;
    }
    options[2] = 0;
    options[3] = 0;
    for (;;) {
        WaitSema(sdfCdImageFd);
        ready = sceCdStStart(lsn, options);
        SignalSema(sdfCdImageFd);
        if (ready != 0) {
            break;
        }
        if (sceCdStatus() == 1) {
            sdfCdWaitTrayClose();
        } else {
            sdfDelayThread(100);
        }
    }
    sdfCdCurrLsn = lsn;
}

void sdfDevSignalPendingSemaphore(void)
{
    if (sdfDiscRequestPending != 0) {
        SignalSema(sdfDiscRequestSemaphore);
        sdfDiscRequestPending = 0;
    }
}

void sdfDevSeekDiscWithRequestGate(s32 lsn)
{
    u8 options[4];
    s32 ready;

    options[0] = 0;
    if (sdfCdImageMode == 1) {
        options[1] = 0;
    } else {
        options[1] = 1;
    }
    options[2] = 0;
    options[3] = 0;
    for (;;) {
        WaitSema(sdfCdImageFd);
        sdfDiscRequestPending = 1;
        WaitSema(sdfDiscRequestSemaphore);
        ready = sceCdStStart(lsn, options);
        SignalSema(sdfCdImageFd);
        if (ready != 0) {
            break;
        }
        if (sceCdStatus() == 1) {
            sdfCdWaitTrayClose();
        } else {
            sdfDelayThread(100);
        }
    }
    sdfCdCurrLsn = lsn;
}

INCLUDE_ASM(const s32, "sdf/sdfCd", func_003E1C00);

INCLUDE_ASM(const s32, "sdf/sdfCd", func_003E1CD0);

INCLUDE_ASM(const s32, "sdf/sdfCd", sdfCdImageSearchFile);

INCLUDE_ASM(const s32, "sdf/sdfCd", func_003E2030);

/* Consume up to size disc-file bytes, retaining a partial sector; return the byte count. */
s32 sdfCdImageRead(void *buff, s32 length) {
    s32 available = sdfCdImageCurrRemain;
    u8 *destination = buff;
    s32 bytes;
    s32 transferBytes;
    s32 t;
    if (available <= 0) {
        return 0;
    }
    if (length > available) {
        length = available;
    }
    if (length == 0) {
        return 0;
    }
    bytes = length;
    transferBytes = sdfCdImageCurrActive;
    if (transferBytes > 0) {
        if (transferBytes > length) {
            transferBytes = length;
        }
        memcpy(destination, sdfCdImageOffset, transferBytes);
        destination += transferBytes;
        sdfCdImageOffset += transferBytes;
        sdfCdImageCurrActive -= transferBytes;
        bytes -= transferBytes;
    }
    if (bytes > 0) {
        transferBytes = bytes;
        if ((EE_ADDR(destination) & 15) == 0) {
            t = transferBytes >> 11;
            t = transferBytes - t * 0x800;
            transferBytes -= t;
            if (transferBytes > 0) {
                sdfCdStRead(transferBytes >> 11, destination);
            }
            if (t > 0) {
                sdfCdStRead(1, sdfCdSectorBuf);
                memcpy(destination + transferBytes, sdfCdSectorBuf, t);
                sdfCdImageOffset = sdfCdSectorBuf + t;
                sdfCdImageCurrActive = 0x800 - t;
            }
        } else {
            while (transferBytes >= 0x800) {
                sdfCdStRead(1, sdfCdSectorBuf);
                memcpy(destination, sdfCdSectorBuf, 0x800);
                transferBytes -= 0x800;
                destination += 0x800;
            }
            if (transferBytes > 0) {
                sdfCdStRead(1, sdfCdSectorBuf);
                memcpy(destination, sdfCdSectorBuf, transferBytes);
                sdfCdImageOffset = sdfCdSectorBuf + transferBytes;
                sdfCdImageCurrActive = 0x800 - transferBytes;
            }
        }
    }
    sdfCdImageCurrRemain = available - length;
    if (sdfCdImageCurrRemain == 0) {
        sdfCdImageClose();
    }
    return length;
}

void sdfCdImageClose(void)
{
    if (sdfCdImageCurrFile != 0) {
        WaitSema(sdfCdImageFd);
        sceCdStStop();
        SignalSema(sdfCdImageFd);
        sdfCdImageCurrFile = NULL;
    }
}

s32 sdfCdImageCheckExist(const char *fname)
{
    s32 lsn;

    return sdfCdImageSearchFile(fname, &lsn, NULL) != 0;
}

s32 sdfCdImageGetFileSize(const char *fname)
{
    s32 lsn;
    sdfCdImageDirEntry_t *entry = sdfCdImageSearchFile(fname, &lsn, NULL);

    if (entry == NULL) {
        return -1;
    }
    return entry->size_;
}

s32 sdfCdImageGetCurrFileSize(void)
{
    sdfCdImageDirEntry_t *file = sdfCdImageCurrFile;

    if (file == NULL) {
        FatalError("file didn't open.");
    }
    return file->size_;
}

/* Read a whole named file into the loader's allocated EE buffer, then close it. */
void sdfCdImageLoadDir(const char *dirFile) {
    s32 fileHandle = sceOpen(dirFile, 1);
    s32 fileBytes;
    void *buffer;
    if (fileHandle < 0) {
        FatalError("%s couldn't open.", dirFile);
    }
    fileBytes = sceLseek(fileHandle, 0, 2);
    sceLseek(fileHandle, 0, 0);
    buffer = malloc(fileBytes);
    sdfCdImageDirData = buffer;
    sceRead(fileHandle, EE_ADDR_S(buffer), fileBytes);
    sceClose(fileHandle);
}

/* Record the disc search path, preload a separate file, and initialize an aligned IOP buffer. */
void sdfCdImageInit(const char *imageFile, const char *dirFile) {
    u32 discFileRecord[12];
    s32 iopHeapAddress;
    dds3ImgPath = imageFile;
    sdfCdImageLoadDir(dirFile);
    if (sceCdSearchFile((sceCdlFILE *)discFileRecord, (const char *)imageFile) == 0) {
        FatalError("%s couldn't open.", imageFile);
    }
    dds3ImgLsn = discFileRecord[0];
    iopHeapAddress = sceSifAllocIopHeap(0x28010);
    sdfCdImageFileName = iopHeapAddress;
    sceCdStInit(0x50, 5, (iopHeapAddress + 15) & -16);
}

INCLUDE_ASM(const s32, "sdf/sdfCd", func_003E24D0);

INCLUDE_SDATA(const s32, "sdf/sdfCd", D_004E4380);

INCLUDE_SDATA(const s32, "sdf/sdfCd", D_004E4388);

INCLUDE_SDATA(const s32, "sdf/sdfCd", D_004E4390);

