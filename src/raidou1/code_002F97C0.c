#include "common.h"
#include "kernel.h"
#include "sdfMemory.h"
#include "fldNme.h"
#include "dds3KernelCore.h"
#include "dds3KernelMain.h"
#include "frFont.h"

INCLUDE_ASM(const s32, "code_002F97C0", fldCallMemGetPtr);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/002FA1E8.c:1, found */
s32 fldCallMemGetPtr2(dds3ProcessID_t *id) {
    fldNmeWork_t *work = sdfMemGetPtr(id->workdata);

    switch (work->unk8) {
    case 1:
        fclCallSetSelect(&work->draw, 0);
        fclLeaf15(&work->draw);
        fldCallMemGetPtr9(id);
        fclCallSetFontColorNo(&work->draw, work->unkC, work->unk30, work->unk10, work->unk34, work->unk38);
        fclFunc6(&work->draw, work->unk10, work->unk3C);
        fldCallMemGetPtr10(id);
        break;
    case 2:
        fclCallSetSelect(&work->draw, 0);
        fclLeaf15(&work->draw);
        fldCallMemGetPtr9(id);
        fclFunc6(&work->draw, work->unk10, work->unk3C);
        fldCallMemGetPtr11(id);
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_002F97C0", fldLeaf28);

void fldCallMemGetPtr5(dds3ProcessID_t *id) {
    fldNmeWork_t *work = sdfMemGetPtr(id->workdata);
    u32 i;

    for (i = 0; i < 2; i++) {
        fclFunc10(&work->list[i]);
    }
    fclFunc4(&work->draw);
    if (work->drawProcess != NULL) {
        dds3KillProcess(work->drawProcess, 0);
    }
    frCallMemFreeDirect(4);
    dds3KernelDebugSign = 1;
    sdfMemFreeDelayedDirect(id->workdata);
}

s32 fldCallMemGetPtr6(dds3ProcessID_t *id, u32 *code) {
    fldNmeWork_t *work = sdfMemGetPtr(id->workdata);
    s32 page;
    s32 index;
    u32 count;
    s32 ret;

    if (work->unkC >= 3) {
        if (work->unkC == 3) {
            titleFunc4(titleFunc6(work->unk38), &page, &index);
            index += work->unk34;
            count = fclLeaf23(page);
            if (index >= count) {
                return 0;
            }
            *code = fclLeaf24(page, index);
            return 1;
        }
    } else {
        ret = titleFunc(work->unkC, work->unk34, work->unk38);
        if (ret != 0) {
            index = nmeLeaf2(work->unkC, work->unk34, work->unk38);
            *code = index;
            return 1;
        }
        return ret;
    }
    return 0;
}
