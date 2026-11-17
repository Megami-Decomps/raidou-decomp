#ifndef EVTPICTURE_H
#define EVTPICTURE_H

#include "common.h"
#include "kernel.h"
#include "libsdf.h"

/* 0x68-byte picture work block. type holds the fade kind in its low half and
 * the layout in its high half; p_now, when set, replaces now2 as the clock. */
typedef struct
{
    u32 flag;
    u32 type;
    sdfTexHandle_t *tex;
    sdfIVector4_t xy;
    sdfIVector4_t uv;
    sdfIVector4_t col;
    sdfIVector4_t ofs_xy;
    s32 start;
    s32 end;
    s32 *p_now;
    s32 now2;
    u32 uid;
    void (*callback)(u32 uid);
    s16 in_timer;
    s16 out_timer;
#ifdef VERSION_RAIDOU2
    s32 ot;
#endif
} evtPictureProcessData_t;

extern char evtCallGetSmallFontFRQ2Data[];
/* Sprite position, uv and colour words the picture draw hands itfMiscDrawSpritePrim. */
extern s32 evtPictureSpritePos[4];
extern s32 evtPictureSpriteUv[4];
extern s32 evtPictureSpriteColor[4];

void evtKillPictureProcess(dds3ProcessID_t *id);
void evtPictureDrawOff(dds3ProcessID_t *id);
void evtPictureDrawOn(dds3ProcessID_t *id);
s32 evtPictureLoadCheckProcess(dds3ProcessID_t *id);
s32 evtPictureSetEndProcess(dds3ProcessID_t *id);

dds3ProcessID_t *StartProcess_evtPicture(DDS3_PRIORITY priority, char *texpath);
dds3ProcessID_t *evtPictureStartProcess4(s32 x, s32 y, u32 priority, s32 start, s32 end, s32 *p_now, u32 type, u32 uid, void (*callback)(u32 uid), const char *texpath);

#endif
