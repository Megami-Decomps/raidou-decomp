#include "common.h"
#include "dds3Basic.h"
#include "dds3obj.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "dds3CameraObjectBasic.h"
#include "dds3ObjectBase.h"
#include "sbtlWork.h"
#include "sBtlUnit.h"
#include "sBtlEff.h"
#include "sBtlSound.h"
#include "sBtlScript.h"
#include "sbtlDMG.h"
#include "sBtlCalc.h"
#include "sBtlSM.h"
#include "sBtlItem.h"
#include "sdatUnit.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "mdlManager.h"
#include "code_r1_0021CE60.h"
#include "sdfDelayedResrc.h"
#include "libsdf.h"
#include "fileManager.h"
#include "sdfFile.h"
#include "dds3Randomize.h"
#include "sdfSound.h"
#include "sbtlSub.h"
#include "sbtlSub_0027ED08.h"
#include "sdfQuat.h"
#include "sBtlSkill.h"
#include "sBtlAI.h"
#include "sBtlDvltk.h"
#include "sbtlActAI.h"
#include "evtUnitManager.h"
#include "dds3UnitObjectBasic.h"
#include "sdk_libc.h"
#include "fpu.h"
#include "ps2asm.h"
#include "dds3KernelCore.h"
#include "dds3KernelMain.h"
#include "fld.h"
#include "sBtlPlayer.h"
#include "dds3PadManager.h"
#include "scrScriptProcess.h"
#include "itfMesManager.h"
#include "sBtlTalk.h"
#include "sdfVu0.h"
#include "newdata.h"
#include "code_r1_0027CBE8.h"
#include "sBtlPanel.h"
#include "AW_XAtkPad.h"
#include "EventBit.h"
#include "fldProcess.h"
#include "sdfMath.h"
#include "sbtlTubeSupport.h"
#include "sdfSystem.h"

s32 sbtlCallFreeAuto(void **dst, s32 size, s32 *src)
{
    s32 raw = 0;
    s32 len = *src;
    void *old;
    void *buf;
    u32 n;

    if (size != 0 && len != size) {
        len = size;
        raw = 1;
    }
    old = *dst;
    if (old != NULL) {
        sdfFreeAuto(old);
    }
    buf = sdfAllocAuto(len);
    *dst = buf;
    if (!raw) {
        memcpy(buf, src + 1, len);
    } else {
        memcpy(buf, src, len);
    }
    n = (u32)(len + 4) >> 4;
    if ((len + 4) & 0xF) {
        return (n + 1) << 4;
    }
    return n << 4;
}

void sbtlCallFileOpen(sbtlFileEntry_t *entry)
{
    sdfFileCmdHandle_t *file = sdfFileOpen(entry->path);
    s32 size = sdfFileLength(file);
    sdfMemHandle_t *handle = sdfMemAllocFromTailDirect(size);
    u8 *buf = sdfMemLock(handle);
    s32 i;

    sdfFileRead(file, buf, size);
    sdfFileClose(file);
    for (i = 0; entry->slot[i].dst != NULL; i++) {
        buf += sbtlCallFreeAuto(entry->slot[i].dst, entry->slot[i].arg, (s32 *)buf);
    }
    sdfMemFreeDirect(handle);
}

SBTLITEM_TBL *sbtlGetItemtbl(s32 id)
{
    return &tbldata[id];
}

INCLUDE_ASM(const s32, "code_002999C0", sbtlSetDropItem);

SBTLITEM_LISTWORK *sbtlCreateItemList(void)
{
    SBTLITEM_LISTWORK *lwk = sdfChipAllocZeroDirect(sizeof(SBTLITEM_LISTWORK));

    lwk->handle = NULL;
    lwk->num = 0;
    return lwk;
}

/* NON_MATCHING: 15 of 74 words match. Register allocation of the item pointer walk differs; same stores. */
#ifdef NON_MATCHING
s32 sbtlEntryItemList(SBTLITEM_LISTWORK *lwk)
{
    /* Retail walks 256 counts from item[], past its 160 entries into stock[]. */
    s16 *item;
    s32 cnt = 0;
    s32 num;
    s32 i;
    sdfMemHandle_t *handle;
    SBTLITEM_TBL *tbl;

    item = DDS3_GBWK->playerData.item;
    for (i = 255; i >= 0; i--) {
        if (*item++ != 0) {
            cnt++;
        }
    }
    if (cnt == 0) {
        lwk->num = 0;
        lwk->handle = NULL;
        return 0;
    }
    num = 0;
    handle = sdfMemAllocDirect(cnt * sizeof(SBTLITEM_DATA));
    lwk->handle = handle;
    lwk->data = sdfMemLock(handle);
    for (i = 0; i < 256; i++) {
        item = &DDS3_GBWK->playerData.item[0] + i;
        if (*item != 0 && ((tbl = sbtlGetItemtbl(i))->usetype & 2) && (tbl->type & 1)) {
            lwk->data[num].itemid = i;
            lwk->data[num].num = *item;
            num++;
        }
    }
    lwk->num = num;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_002999C0", sbtlEntryItemList);
#endif

/* NON_MATCHING: 17 of 71 words match. Register allocation of the item pointer walk differs; same stores. */
#ifdef NON_MATCHING
s32 sbtlEntryItemList_Type(SBTLITEM_LISTWORK *lwk, u16 type)
{
    /* Retail walks 256 counts from item[], past its 160 entries into stock[]. */
    s16 *item;
    s32 cnt = 0;
    s32 num;
    s32 i;
    sdfMemHandle_t *handle;

    item = DDS3_GBWK->playerData.item;
    for (i = 255; i >= 0; i--) {
        if (*item++ != 0) {
            cnt++;
        }
    }
    if (cnt == 0) {
        lwk->num = 0;
        lwk->handle = NULL;
        return 0;
    }
    num = 0;
    handle = sdfMemAllocDirect(cnt * sizeof(SBTLITEM_DATA));
    lwk->handle = handle;
    lwk->data = sdfMemLock(handle);
    for (i = 0; i < 256; i++) {
        item = &DDS3_GBWK->playerData.item[0] + i;
        if (*item != 0 && (sbtlGetItemtbl(i)->type & type)) {
            lwk->data[num].itemid = i;
            lwk->data[num].num = *item;
            num++;
        }
    }
    lwk->num = num;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_002999C0", sbtlEntryItemList_Type);
#endif

s32 sbtlReleaseItemList(SBTLITEM_LISTWORK *lwk)
{
    if (lwk->handle != NULL) {
        sdfMemFreeDirect(lwk->handle);
        lwk->handle = NULL;
    }
    lwk->num = 0;
    return 1;
}
