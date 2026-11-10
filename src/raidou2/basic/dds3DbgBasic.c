#include "common.h"

/* Declarations from smt3-decomp src/smt3/basic/dds3DbgBasic.c */
#include "common.h"

#include "sdk_libgcc.h"

#include "basic.h"

#include "kernel.h"

#include "newdata.h"

#include "ps2asm.h"

extern sdfGrOt_t sdfGrOtTable[];

extern s8 dds3PadButtonState[];

extern void *sdfDevPrintf(s32 aX, s32 aY, s32 aZ, s32 aAttr, const char *aStr, ...);

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);

extern void *sdfGrAllocPacketBuff(u32 aSize);

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern dds3Basic_t *dds3GetActiveScene(void);

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern s32 dds3SceneBasicExistBasic(dds3Basic_t *scene, dds3Basic_t *target);

extern void dds3ConsPanelTagLink(sdfDrawTag_t *tag, s32 x, s32 y, s32 sx, s32 sy);

extern saddr_t evtSceneDispProcess(void *id);

extern s32 DispBaseData(dds3Basic_t *basic, s32 x, s32 y, sdfDrawTag_t *tag);

extern char dds3BasicPosLabel[];

extern char dds3BasicRotLabel[];

extern char dds3BasicSclLabel[];

s32 DispBaseData(dds3Basic_t *node, s32 x, s32 y, sdfDrawTag_t *list);

u32 DispObjectBaseData(dds3Basic_t *basic, s32 x, s32 y, sdfDrawTag_t *tag);

saddr_t dds3DebugBasic(dds3ProcessID_t *id);

extern char dds3BasicPosLabel[8];

extern char dds3BasicRotLabel[8];

extern char dds3BasicSclLabel[8];
/* End of declarations from src/smt3/basic/dds3DbgBasic.c */

s32 DispBaseData(dds3Basic_t *node, s32 x, s32 y, sdfDrawTag_t *list) {
    dds3PoseSet_t *transform;
    s32 labelX;
    s32 valueX;
    s32 positionY;
    s32 rotationY;
    s32 scaleY;
    s32 lines;

    x *= 12;
    labelX = (x << 4) + 0x7000;
    sdfDrawTagLink(list, EE_ADDR(sdfDevPrintf(labelX,
        (y++ * 6 << 4) + 0x7900, 0xFEFFFF, 0, "ID    : 0x%05X", node->serialID)));
    sdfDrawTagLink(list, EE_ADDR(sdfDevPrintf(labelX,
        (y++ * 6 << 4) + 0x7900, 0xFEFFFF, 0, "NAME  : %s", node->name)));
    transform = node->poseset;
    lines = 2;
    if (transform != NULL) {
        y++;
        positionY = (y * 6 << 4) + 0x7900;
        sdfDrawTagLink(list, EE_ADDR(sdfDevPrintf(labelX,
            positionY, 0xFEFFFF, 0, dds3BasicPosLabel)));
        valueX = ((x + 84) << 4) + 0x7000;
        sdfDrawTagLink(list, EE_ADDR(sdfDevPrintf(valueX,
            positionY, 0xFEFFFF, 0xE, "%3.2f %3.2f %3.2f",
            transform->pos.x, transform->pos.y, transform->pos.z)));
        y += 2;
        rotationY = (y * 6 << 4) + 0x7900;
        sdfDrawTagLink(list, EE_ADDR(sdfDevPrintf(labelX,
            rotationY, 0xFEFFFF, 0, dds3BasicRotLabel)));
        sdfDrawTagLink(list, EE_ADDR(sdfDevPrintf(valueX,
            rotationY, 0xFEFFFF, 0xE, "%3.2f %3.2f %3.2f %3.2f",
            transform->rot.x, transform->rot.y, transform->rot.z, transform->rot.w)));
        y += 2;
        scaleY = (y * 6 << 4) + 0x7900;
        sdfDrawTagLink(list, EE_ADDR(sdfDevPrintf(labelX,
            scaleY, 0xFEFFFF, 0, dds3BasicSclLabel)));
        sdfDrawTagLink(list, EE_ADDR(sdfDevPrintf(valueX,
            scaleY, 0xFEFFFF, 0xE, "%3.1f %3.1f %3.1f",
            transform->scl.x, transform->scl.y, transform->scl.z)));
        lines = 8;
    }
    return lines;
}

u32 DispObjectBaseData(dds3Basic_t *basic, s32 x, s32 y, sdfDrawTag_t *tag) {
    dds3ObjectBaseControlWorkData_t *handle;
    void *command;

    handle = dds3GetBaseControlWorkData(basic);
    command = sdfDevPrintf((x * 3 << 6) + 0x7000,
                                           (y * 3 << 5) + 0x7900,
                                           0xFEFFFF,
                                           0,
                                           "FLAG  : 0x%08X",
                                           handle->flag);
    sdfDrawTagLink(tag, EE_ADDR(command));
    return 1;
}

INCLUDE_ASM(const s32, "basic/dds3DbgBasic", dds3DebugBasic);
INCLUDE_SDATA(const s32, "basic/dds3DbgBasic", D_004DE5B0);

INCLUDE_SDATA(const s32, "basic/dds3DbgBasic", D_004DE5B8);

INCLUDE_SDATA(const s32, "basic/dds3DbgBasic", D_004DE5C0);

