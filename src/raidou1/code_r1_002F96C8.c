#include "common.h"
#include "dds3KernelCore.h"
#include "sdfMemory.h"
#include "fldNme.h"

extern const char fldStrFontNameEntFnt[];
extern const char fldStrNmeProcess[];
extern const char fldStrNmeDrawProcess[];
extern s32 dds3KernelDebugSign;
extern s32 fldCallMemGetPtr(dds3ProcessID_t *id);
extern s32 fldCallMemGetPtr2(dds3ProcessID_t *id);
extern s32 fldCallMemGetPtr5(dds3ProcessID_t *id);
extern s32 titleStartSub(s32 arg);
extern s32 titleStartSub2(s32 arg);

dds3ProcessID_t *fldNmeProcess(void) {
    sdfMemHandle_t *mem = sdfMemAllocDirect(sizeof(fldNmeWork_t));
    dds3ProcessID_t *process = dds3StartProcess(fldStrNmeProcess, 0x3FF, 0, 0, fldCallMemGetPtr, fldCallMemGetPtr5, mem);
    fldNmeWork_t *work = sdfMemGetPtr(mem);

    work->unk0 = 0;
    work->drawProcess = dds3StartProcess(fldStrNmeDrawProcess, 0x2B07, 0, 0, fldCallMemGetPtr2, NULL, mem);
    work->unk34 = 0;
    work->unk38 = 0;
    work->unk3C = 0;
    work->unk14 = 0;
    work->unk40 = 0;
    work->unk30 = 0;
    work->unk10 = 0;
    work->unkC = 0;
    work->unk44 = titleStartSub(0);
    work->unk48 = titleStartSub2(work->unkC);
    work->font = fileLoad(fldStrFontNameEntFnt);
    work->unk8 = 0;
    dds3KernelDebugSign = 0;
    return process;
}

INCLUDE_RODATA(const s32, "code_r1_002F96C8", fldStrNmeProcess);

INCLUDE_RODATA(const s32, "code_r1_002F96C8", fldStrNmeDrawProcess);

INCLUDE_RODATA(const s32, "code_r1_002F96C8", fldStrFontNameEntFnt);

