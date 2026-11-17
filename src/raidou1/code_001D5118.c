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

extern dds3ProcessID_t *slLoadProcId;
extern s8 slLoadKillFlag;

/* body: matched in smt3 @ 9401f79e: src/smt3/code_0029B780.c:93, found, same function 1.00 (words 12/12, calls 1.00, shape 1.00) */
void slKillLoadProcess(void)
{
    if (slLoadProcId != NULL) {
        dds3KillProcess(slLoadProcId, 1);
        slLoadProcId = NULL;
        slLoadKillFlag = 0;
    }
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_0028A150.c:527, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s8 fileMenuTaskIsAlive(void) {
    return slLoadKillFlag;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_0028A150.c:531, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void mnuFormatSaveSlotHeaderName(void) {
}
