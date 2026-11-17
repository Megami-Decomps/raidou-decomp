#include "common.h"
#include "sdk_libgcc.h"
#include "basic.h"
#include "kernel.h"
#include "newdata.h"
#include "ps2asm.h"
#include "sdfDraw.h"
#include "sdfDevCons.h"
#include "kernel_draw.h"
#include "dds3PadManager.h"
#include "evtDebug.h"
#include "dds3KernelCore.h"
#include "dds3SceneBasic.h"
#include "dds3WorldBasic.h"
#include "sdfPacketBuff.h"
#include "dds3KernelMisc.h"

extern s32 DispBaseData(dds3Basic_t *basic, s32 x, s32 y, sdfDrawTag_t *tag);
extern char dds3BasicPosLabel[];
extern char dds3BasicRotLabel[];
extern char dds3BasicSclLabel[];
s32 DispBaseData(dds3Basic_t *node, s32 x, s32 y, sdfDrawTag_t *list);
u32 DispObjectBaseData(dds3Basic_t *basic, s32 x, s32 y, sdfDrawTag_t *tag);
extern char dds3BasicPosLabel[8];
extern char dds3BasicRotLabel[8];
extern char dds3BasicSclLabel[8];

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

/* NON_MATCHING: 41 of 73 words match.  */
#ifdef NON_MATCHING
dds3ProcessFunc_t dds3DebugBasic(dds3ProcessID_t *id) {
    dds3Basic_t *basic;
    sdfDrawTag_t *list;
    sdfDrawTag_t *panel;
    s32 width;
    u32 kind;
    s32 noScene;
    dds3ProcessFunc_t result;

    noScene = dds3GetActiveScene() == NULL;
    result = (dds3ProcessFunc_t)-1;
    if (!noScene) {
        basic = dds3GetProcessData(id);
        if (dds3SceneBasicExistBasic(dds3GetActiveScene(), basic) == 0) {
            return evtSceneDispProcess;
        }
        list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
        sdfDrawTagInit(list);
        width = DispBaseData(basic, 3, 10, list) + 11;
        kind = basic->type >> 24;
        if (kind < 0xA && kind >= 4) {
            width += DispObjectBaseData(basic, 3, width, list);
        }
        panel = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
        sdfDrawTagInit(panel);
        dds3ConsPanelTagLink(panel, 36, 114, 35, width - 9);
        dds3Data10.regFunc(&dds3Data10, panel);
        dds3Data10.regFunc(&dds3Data10, list);
        if (dds3PadButtonState[0x31] >= 0) {
            return NULL;
        }
        return evtSceneDispProcess;
    }
    return result;
}
#else
INCLUDE_ASM(const s32, "basic/dds3DbgBasic", dds3DebugBasic);
#endif
char dds3BasicPosLabel[8] = "POS  :";

char dds3BasicRotLabel[8] = "ROT  :";

char dds3BasicSclLabel[8] = "SCL  :";
