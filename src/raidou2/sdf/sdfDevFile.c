#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "sdfMat.h"
#include "sdfTex.h"

void *sdfDevLoadFile(const char *aFileName, u32 *aOutData, u32 *aOutSize, s32 aAlign);

typedef struct
{
    u8 unk00[0x10];
    s32 resolveOffset;
    s32 resolveCount;
    u8 unk18[8];
} sdfBinaryHeader_t;

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

typedef struct sdfTextureOffsetListEntry_t {
    s32 textureHeaderOffset;
} sdfTextureOffsetListEntry_t;

typedef struct sdfTextureOffsetListHeader_t {
    u8 unknown00[0x10];
    s32 textureCount;
} sdfTextureOffsetListHeader_t;

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

u32 sdfMemLock(sdfMemHandle_t *aHandle);

#include "sdk_libc.h"

void sdfMemUnlock(sdfMemHandle_t *aHandle);
sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);
void sdfFileClose(void *aHandle);
u32 sdfFileLength(void *aHandle);

/* Open a path using the shared reply callback and wait for its initial completion. */ sdfFileCmdHandle_t *sdfFileOpen(const char *path);

s32 sdfFileRead(void *aHandle, void *aPtr, s32 aLeng);
void *sdfDevLoadFile(const char *name, u32 *outData, u32 *outSize, s32 extra);
sdfItemArray_t *sdfTexSetTB(const sdfTextureOffsetListHeader_t *aData);

/* Read a named file through the dev RPC into a freshly allocated block; returns the block's handle.
 * outData receives the block address, outSize the file size; without outData the block is released. */
void *sdfDevLoadFile(const char *name, u32 *outData, u32 *outSize, s32 extra) {
    sdfFileCmdHandle_t *state = sdfFileOpen(name);
    s32 size = sdfFileLength(state);
    sdfMemHandle_t *handle = sdfMemAllocDirect(size + extra);
    u32 address = sdfMemLock(handle);
    sdfFileRead(state, EE_ADDR_AS(void *, address), size);
    sdfFileClose(state);
    if (outData != NULL) {
        *outData = address;
    } else {
        sdfMemUnlock(handle);
    }
    if (outSize != NULL) {
        *outSize = size;
    }
    return handle;
}

void *sdfDevLoadFileEx(const char *aFileName, u32 *aOutData, u32 *aOutSize)
{
    return sdfDevLoadFile(aFileName, aOutData, aOutSize, 0);
}

INCLUDE_ASM(const s32, "sdf/sdfDevFile", sdfDevLoadTex);

sdfItemArray_t *sdfTexSetTB(const sdfTextureOffsetListHeader_t *aData) {
    s32 i = 0;
    s32 count = aData->textureCount;
    sdfItemArray_t *handle = sdfTexListNew(count);
    const sdfTextureOffsetListEntry_t *entry;
    if (count != i) {
        entry = (const sdfTextureOffsetListEntry_t *)((const u8 *)aData + sizeof(*aData));
        do {
            i++;
            sdfTexListAdd(handle, sdfTexSet((TMX_HEADER_t *)(
                (void *)((const u8 *)aData + entry->textureHeaderOffset))));
            entry++;
        } while (i != count);
    }
    return handle;
}

INCLUDE_ASM(const s32, "sdf/sdfDevFile", sdfCallDevLoadFileEx);

INCLUDE_ASM(const s32, "sdf/sdfDevFile", sdfRelocatePackedResourceWordsFromHeader);

INCLUDE_ASM(const s32, "sdf/sdfDevFile", sdfReadPackedResourceAndRelocateHeader);

INCLUDE_ASM(const s32, "sdf/sdfDevFile", sdfRelocatePackedResourcePayload);

INCLUDE_ASM(const s32, "sdf/sdfDevFile", sdfLoadPackedResourceWithRelocatedPayload);
