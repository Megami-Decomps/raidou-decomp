#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "fldProcess.h"
#include "mdlManager.h"
#include "fileManager.h"
#include "sdfMemory.h"
#include "sdfTex.h"
#include "sdk_libc.h"
#include "sdfSound.h"
#include "kernel_draw.h"
#include "dds3KernelDraw.h"
#include "dds3KernelMain.h"
#include "effBlur_Filter.h"
#include "fldEnc.h"
#include "fldDraw.h"
#include "sdf2D.h"
#include "sdfPrim.h"
#include "sdfPacketBuff.h"
#include "sdfDraw.h"
#include "sdf_packet_list.h"
#include "fld.h"
#include "fldNpcPath.h"
#include "fpu.h"
#include "ps2asm.h"
#include "sdfMath.h"
#include "sdfQuat.h"
#include "code_00174790.h"

INCLUDE_ASM(const s32, "code_00175348", mapLeaf30);

INCLUDE_ASM(const s32, "code_00175348", mapNpcPathControlGetPos);

INCLUDE_ASM(const s32, "code_00175348", mapNpcPathControlGetRot);

INCLUDE_ASM(const s32, "code_00175348", mapCallGetElement);

s32 mapNpcPathTaskSub(mapNpcPathControl_t *pControl) {
    u32 flag = pControl->flag;
    s32 ret = 1;

    if (flag & 8) {
        return 1;
    }
    if (pControl->mode == 0) {
        if (pControl->frameMax > pControl->frame) {
            pControl->frame += 1.0f;
        } else if (flag & 0x20) {
            pControl->frame = 0.0f;
        } else {
            ret = 0;
        }
    } else if (pControl->mode == 1) {
        if (pControl->frame > 0.0f) {
            pControl->frame -= 1.0f;
        } else if (flag & 0x20) {
            pControl->frame = pControl->frameMax;
        } else {
            ret = 0;
        }
    }
    return ret;
}

INCLUDE_ASM(const s32, "code_00175348", mapLeaf31);

INCLUDE_ASM(const s32, "code_00175348", mapLeaf32);

INCLUDE_ASM(const s32, "code_00175348", mapLeaf33);

INCLUDE_ASM(const s32, "code_00175348", mapLeaf34);

INCLUDE_ASM(const s32, "code_00175348", mapLeaf35);

INCLUDE_ASM(const s32, "code_00175348", mapLeaf36);

INCLUDE_ASM(const s32, "code_00175348", mapObjTaskSub);

INCLUDE_ASM(const s32, "code_00175348", mapLeaf37);

INCLUDE_ASM(const s32, "code_00175348", mapFileLoadSub9);

INCLUDE_ASM(const s32, "code_00175348", mapFileLoadSub10);

INCLUDE_ASM(const s32, "code_00175348", mapLeaf38);

INCLUDE_RODATA(const s32, "code_00175348", mapCallGetElementTable);

