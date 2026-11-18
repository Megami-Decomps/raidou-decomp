#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "sdfMemory.h"
#include "sdfTex.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "evtEventViewer.h"
#include "scr.h"
#include "evtCutIn.h"
#include "evtPicture.h"
#include "evt.h"
#include "evtTmxPreLoad.h"
#include "code_001EB528.h"

void slKillLoadProcess(void)
{
    if (slLoadProcId != NULL) {
        dds3KillProcess(slLoadProcId, 1);
        slLoadProcId = NULL;
        slLoadKillFlag = 0;
    }
}

INCLUDE_ASM(const s32, "code_001EB528", fileMenuTaskIsAlive);

INCLUDE_ASM(const s32, "code_001EB528", mnuFormatSaveSlotHeaderName);

INCLUDE_ASM(const s32, "code_001EB528", slBASLUSSmn);
