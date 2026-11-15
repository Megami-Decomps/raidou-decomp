#include "common.h"
#include "facility.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "sdfFrame.h"
#include "sdfMemory.h"
#include "sdfTex.h"

sdfTexHandle_t *fclLoadTexture(const char *pFileName);

typedef struct
{
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
} fclRotPoint_t;

typedef struct
{
    s32 x;
    s32 y;
    s32 z;
    u16 width;
    u16 height;
    s32 color;
    u8 speed;
    u8 reserved15[3];
    s32 timer;
    s32 timer2;
    s32 flags;
} fclPhasePanel_t;

sdfTexHandle_t *fclLoadTexture(const char *pFileName)
{
    void *data;
    s32 size;
    void *file;
    sdfTexHandle_t *handle;

    file = sdfDevLoadFileEx(pFileName, (u32 *)&data, &size);
    handle = sdfTexSet(data);
    sdfMemFreeDirect(file);
    return handle;
}
