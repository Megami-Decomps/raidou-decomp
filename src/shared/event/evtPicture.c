#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "sdfFrame.h"
#include "evtPicture.h"
#include "dds3KernelCore.h"
#include "fileManager.h"
#include "itfMisc.h"
#include "sdfTex.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "sdk_libc.h"
#include "dds3KernelDraw.h"
#include "frFont.h"
#include "kernel_draw.h"
#include "sdfPacketBuff.h"
#include "sdfDraw.h"

/* One 0x30-byte row of the eight-entry priority table picpri_Process draws. */
typedef struct
{
    void *tex;
    s32 ot;
    f32 x0;
    f32 y0;
    f32 x1;
    f32 y1;
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
    s32 alpha;
    s32 pri;
} evtPicturePriTbl_t;

extern char evtPictureProcName[];

/* The four GS colour components itfMiscDrawSpritePrim packs; only alpha is set
 * per sprite. */
typedef struct
{
    u32 components[4];
} evtPictureColor_t;


void evtPictureDrawOn(dds3ProcessID_t *id);
void evtPictureDrawOff(dds3ProcessID_t *id);
void evtKillPictureProcess(dds3ProcessID_t *id);
evtPictureProcessData_t *InitPictureProcessData(void);
void LoadTexture(evtPictureProcessData_t *data, const char *texpath);
s32 GetNowFrame(evtPictureProcessData_t *data);
void evtFunc(evtPictureProcessData_t *data);
void evtCallGetSmallFontFRQ2(evtPictureProcessData_t *data);
s32 evtPictureProcess1(dds3ProcessID_t *id);
void evtPictureShutdown(dds3ProcessID_t *id);
void LoadTexCallback(fileRequest_t *file, evtPictureProcessData_t *data);
fileRequest_t *evtPictureLoadStartTexFile(const char *texpath, evtPictureProcessData_t *data);
dds3ProcessID_t *evtStartPictureProcess2(u32 priority, const char *texdata);
extern char evtPictureProcName[8];

void evtPictureDrawOn(dds3ProcessID_t *id)
{
    evtPictureProcessData_t *work = dds3GetProcessData(id);
    work->flag |= 1;
}

void evtPictureDrawOff(dds3ProcessID_t *id)
{
    evtPictureProcessData_t *work = dds3GetProcessData(id);
    work->flag &= ~1;
}

void evtKillPictureProcess(dds3ProcessID_t *id)
{
    dds3KillProcess(id, 1);
}

evtPictureProcessData_t *InitPictureProcessData(void)
{
    evtPictureProcessData_t *data = sdfChipAllocDirect(sizeof(evtPictureProcessData_t));

    memset(data, 0, sizeof(evtPictureProcessData_t));
    data->flag = 0;
    data->tex = NULL;
    data->xy.pm[0] = 0;
    data->xy.pm[1] = 0;
    data->xy.pm[2] = 512 << 4;
    data->xy.pm[3] = 448 << 3;
    data->uv.pm[0] = 0;
    data->uv.pm[1] = 0;
    data->uv.pm[2] = 512 << 4;
    data->uv.pm[3] = 448 << 4;
    data->col.pm[0] = 0x80;
    data->col.pm[1] = 0x80;
    data->col.pm[2] = 0x80;
    data->col.pm[3] = 0x7F;
#ifdef VERSION_RAIDOU2
    data->ot = dds3IsBistaZBufferClear() ? 98 : 104;
#endif
    return data;
}

INCLUDE_ASM(const s32, "event/evtPicture", LoadTexture);

s32 GetNowFrame(evtPictureProcessData_t *data)
{
    if (data->p_now != NULL) {
        return *data->p_now;
    }
    return data->now2;
}

void evtFunc(evtPictureProcessData_t *data)
{
    s32 now;
    s32 alpha;
    s32 ofs[4];
    s32 i;

    now = GetNowFrame(data);
    alpha = 256;
    if (data->type == 0) {
        return;
    }
    if (data->in_timer < 5) {
        if ((data->type >> 16) < 3) {
            data->in_timer++;
            alpha = (data->in_timer << 8) / 5;
        }
    } else if (data->start != data->end || data->end != now) {
        switch (data->type & 0xFFFF) {
        case 1:
            if (data->end - now + 4 < 5) {
                if (data->out_timer < 5) {
                    data->out_timer++;
                }
                alpha = ((5 - data->out_timer) << 8) / 5;
            } else {
                data->out_timer = 0;
            }
            break;
        case 2:
            return;
        }
    }
    switch (data->type >> 16) {
    case 1:
        ofs[0] = -15;
        ofs[1] = 150;
        break;
    case 2:
        ofs[1] = 150;
        ofs[0] = data->tex->width + 15;
        break;
    default:
        ofs[0] = 0;
        ofs[1] = 0;
        break;
    }
    ofs[2] = ofs[0] - data->tex->width;
    ofs[3] = ofs[1] - data->tex->height;
    for (i = 0; i < 4; i++) {
        data->ofs_xy.pm[i] = ofs[i] * (256 - alpha) / 256;
    }
    data->col.pm[3] = alpha / 2;
}

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "event/evtPicture", evtCutInCreateSub);

#endif
#ifdef VERSION_RAIDOU1
void evtCallGetSmallFontFRQ2(evtPictureProcessData_t *data)
{
    frFontHandle_t *frq;
    sdfDrawTag_t *tag;
    sdfIVector4_t xy;
    sdfTexHandle_t *tex;
    s32 w;
    s32 h;
    sdfGrOt_t *ot;

    frq = frwGetSmallFontFRQ2(0, 0, 0, 0xA09DC380, evtCallGetSmallFontFRQ2Data, NULL);
    frSetFontColor(frq, 0);
    frDrawFontOt(frq, 1, 103);
    frDestroyFontRequestAfter(frq);
    tag = (sdfDrawTag_t *)sdfGrAllocPacketBuff(32);
    sdfDrawTagInit(tag);
    itfMiscDrawAlphaPrim(tag, 0, 0);
    xy.pm[0] = data->xy.pm[0] + data->ofs_xy.pm[0] * 16;
    xy.pm[1] = data->xy.pm[1] + data->ofs_xy.pm[1] * 8;
    xy.pm[2] = data->xy.pm[2] + data->ofs_xy.pm[2] * 16;
    xy.pm[3] = data->xy.pm[3] + data->ofs_xy.pm[3] * 8;
    if (data->flag & 0x10) {
        tex = data->tex;
        w = tex->width / 2 * 16;
        h = tex->height / 2 * 8;
        xy.pm[0] -= w;
        xy.pm[1] -= h;
        xy.pm[2] -= w;
        xy.pm[3] -= h;
    } else {
        tex = data->tex;
    }
    itfMiscDrawSpritePrim(&xy, &data->uv, &data->col, 4095, tex, 0, tag);
    ot = &sdfGrOtTable[103];
    ot->regFunc(ot, tag);
}
#else
INCLUDE_ASM(const s32, "event/evtPicture", evtCallGetSmallFontFRQ2);
#endif

s32 evtPictureProcess1(dds3ProcessID_t *id)
{
    evtPictureProcessData_t *data = dds3GetProcessData(id);
    s32 now;
    s32 margin;

    if (!(data->flag & 1)) {
        return 0;
    }
    if (data->tex != NULL) {
        if (data->flag & 8) {
            return -1;
        }
        evtFunc(data);
        evtCallGetSmallFontFRQ2(data);
    }
    if (data->type != 0) {
        now = GetNowFrame(data);
        if (now < data->start) {
            data->flag |= 8;
        }
        margin = 5;
        if ((u16)data->type != 1) {
            margin = 0;
        }
        if (!(data->flag & 2)) {
            if (now >= data->end + margin) {
                data->flag |= 8;
            }
        } else {
            if (data->end + margin < now) {
                data->flag |= 8;
            }
            if (data->flag & 4) {
                data->now2++;
            }
        }
    }
    return 0;
}

void evtPictureShutdown(dds3ProcessID_t *id)
{
    evtPictureProcessData_t *data = dds3GetProcessData(id);

    if (data->tex != NULL) {
        sdfTexFreeDelayed(data->tex);
        data->tex = NULL;
    }
    if (data->callback != NULL) {
        data->callback(data->uid);
    }
    sdfChipFreeDirect(data);
}

dds3ProcessID_t *evtStartPictureProcess2(u32 priority, const char *texdata)
{
    evtPictureProcessData_t *work = InitPictureProcessData();

    LoadTexture(work, texdata);
    return dds3StartProcess(evtPictureProcName, priority, 1, 1, evtPictureProcess1, (dds3ProcessFunc_t)evtPictureShutdown, work);
}

INCLUDE_ASM(const s32, "event/evtPicture", evtStartPictureProcess);

void LoadTexCallback(fileRequest_t *file, evtPictureProcessData_t *data)
{
    sdfMemHandle_t *mem = fileGetMemHandle(file);

    data->tex = sdfTexSet(sdfMemLock(mem));
    sdfMemFreeDirect(mem);
    fileHandleFree(file);
    data->xy.pm[2] = data->xy.pm[0] + data->tex->width * 16;
    data->xy.pm[3] = data->xy.pm[1] + data->tex->height * 8;
    data->uv.pm[0] = 0;
    data->uv.pm[1] = 0;
    data->uv.pm[2] = data->tex->width * 16;
    data->uv.pm[3] = data->tex->height * 16;
}

fileRequest_t *evtPictureLoadStartTexFile(const char *texpath, evtPictureProcessData_t *data)
{
    data->tex = NULL;
    return fileLoadEx(texpath, 0, EE_ADDR(LoadTexCallback), EE_ADDR(data));
}

s32 evtPictureLoadCheckProcess(dds3ProcessID_t *id)
{
    evtPictureProcessData_t *data = dds3GetProcessData(id);

    return data->tex != NULL;
}

INCLUDE_ASM(const s32, "event/evtPicture", evtCallGetProcessData17);

INCLUDE_ASM(const s32, "event/evtPicture", evtCallGetProcessData18);

INCLUDE_ASM(const s32, "event/evtPicture", evtPictureSetSize);

INCLUDE_ASM(const s32, "event/evtPicture", evtPictureSetEndProcess);

INCLUDE_ASM(const s32, "event/evtPicture", StartProcess_evtPicture);

dds3ProcessID_t *evtPictureStartProcess4(s32 x, s32 y, u32 priority, s32 start, s32 end, s32 *p_now, u32 type, u32 uid, void (*callback)(u32 uid), const char *texpath)
{
    evtPictureProcessData_t *data = InitPictureProcessData();

    data->flag |= 0x11;
    data->xy.pm[0] = x;
    data->xy.pm[1] = y;
    data->start = start;
    data->end = end;
    data->p_now = p_now;
    data->type = type;
    data->uid = uid;
    data->callback = callback;
    evtPictureLoadStartTexFile(texpath, data);
    return dds3StartProcess(evtPictureProcName, priority, 0, 0, evtPictureProcess1, (dds3ProcessFunc_t)evtPictureShutdown, data);
}
#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "event/evtPicture", evtCallGetSmallFontFRQ2Data);
#else
INCLUDE_SDATA(const s32, "event/evtPicture", evtData28);

INCLUDE_SDATA(const s32, "event/evtPicture", evtCallGetSmallFontFRQ2Data2);
#endif

char evtPictureProcName[8] = "Picture";

u16 skyanime = 0;
