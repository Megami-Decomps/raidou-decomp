#include "common.h"
#include "kernel.h"
#include "dds3KernelCore.h"
#include "code_r1_001CC1D0.h"
#include "jmsInit.h"
#include "gohMain.h"
#include "code_00346258.h"
#include "gohInit.h"

INCLUDE_ASM(const s32, "code_00349B80", TalkSelCalcProcess);

dds3ProcessFunc_t TalkSelDrawProcess(dds3ProcessID_t *PID) {
    jmsData_t *work = dds3GetProcessData(PID);

    DrawModel(work, jmsGetCursorPosID() - 0x15);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t TalkSelUpdateProcess(dds3ProcessID_t *PID) {
    jmsData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

u32 TalkInit(dds3ProcessID_t *task) {
    gohData_t *work = dds3GetProcessData(task);

    work->unk90[5] = 2;
    return 1;
}

INCLUDE_ASM(const s32, "code_00349B80", TalkDestroy);

INCLUDE_ASM(const s32, "code_00349B80", TalkCalcProcess);

dds3ProcessFunc_t shpCallGetProcessData(dds3ProcessID_t *PID) {
    jmsData_t *work = dds3GetProcessData(PID);

    DrawModel(work, -1);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t shpCallGetProcessData3(dds3ProcessID_t *PID) {
    jmsData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00349B80", CyousaSelInit);

INCLUDE_ASM(const s32, "code_00349B80", CyousaSelDestroy);

INCLUDE_ASM(const s32, "code_00349B80", shpUpdateDBOOK);

INCLUDE_ASM(const s32, "code_00349B80", CyousaSelDrawProcess);

dds3ProcessFunc_t CyousaSelUpdateProcess(dds3ProcessID_t *PID) {
    jmsData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00349B80", shpLeaf11);

INCLUDE_ASM(const s32, "code_00349B80", jmsLeaf14);

INCLUDE_ASM(const s32, "code_00349B80", CyousaInit);

dds3ProcessFunc_t jmsCallGetProcessData12(dds3ProcessID_t *PID) {
    jmsData_t *work = dds3GetProcessData(PID);

    DrawModel(work, -1);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t jmsReportReadFlagUpdate(dds3ProcessID_t *PID) {
    jmsData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00349B80", CyousaDestroy);

INCLUDE_ASM(const s32, "code_00349B80", jmsLeaf15);

INCLUDE_ASM(const s32, "code_00349B80", CyousaCalcProcess);

dds3ProcessFunc_t jmsCallGetProcessData13(dds3ProcessID_t *PID) {
    jmsData_t *work = dds3GetProcessData(PID);

    DrawModel(work, -1);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t jmsCallGetProcessData14(dds3ProcessID_t *PID) {
    jmsData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00349B80", jmsFunc38);

INCLUDE_ASM(const s32, "code_00349B80", jmsFunc39);

INCLUDE_ASM(const s32, "code_00349B80", jmsCallGetProcessData15);

INCLUDE_ASM(const s32, "code_00349B80", jmsCallProcessStartJmsMenu);

INCLUDE_ASM(const s32, "code_00349B80", jmsCallGetProcessData16);

INCLUDE_ASM(const s32, "code_00349B80", GetDrawText);

INCLUDE_ASM(const s32, "code_00349B80", CyousaDrawProcess);

dds3ProcessFunc_t CyousaUpdateProcess(dds3ProcessID_t *PID) {
    jmsData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00349B80", CheckMahjanError);

INCLUDE_ASM(const s32, "code_00349B80", MahjanInit);

INCLUDE_ASM(const s32, "code_00349B80", MahjanDestroy);

INCLUDE_ASM(const s32, "code_00349B80", MahjanCalcProcess);

dds3ProcessFunc_t jmsCallGetProcessData(dds3ProcessID_t *PID) {
    jmsData_t *work = dds3GetProcessData(PID);

    DrawModel(work, -1);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

INCLUDE_ASM(const s32, "code_00349B80", MahjanUpdateProcess);

INCLUDE_ASM(const s32, "code_00349B80", EndProcess_jmsMain);

INCLUDE_ASM(const s32, "code_00349B80", jmsDrawBtnIconSub);

INCLUDE_ASM(const s32, "code_00349B80", jmsFunc40);

INCLUDE_ASM(const s32, "code_00349B80", jmsFunc41);

INCLUDE_ASM(const s32, "code_00349B80", jmsDrawCyousaPanelSub);

/* body: matched in raidou @ 88f0d54: src/raidou1/TitleMain.c:20, match, same function 1.00 (words 9/9, calls 1.00, shape 1.00) */
s32 jmsLeaf16(s32 value, s32 low, s32 high) {
    if (value >= low && value < high) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_00349B80", jmsFunc42);

INCLUDE_ASM(const s32, "code_00349B80", jmsFunc43);

INCLUDE_ASM(const s32, "code_00349B80", jmsFunc4);

INCLUDE_ASM(const s32, "code_00349B80", DrawCyousaPanel);

INCLUDE_ASM(const s32, "code_00349B80", DrawBtnIcon);

INCLUDE_ASM(const s32, "code_00349B80", jmsFunc44);

INCLUDE_ASM(const s32, "code_00349B80", DrawModel);

INCLUDE_RODATA(const s32, "code_00349B80", jmsFunc4Table);

INCLUDE_RODATA(const s32, "code_00349B80", jmsFunc44Table);

INCLUDE_RODATA(const s32, "code_00349B80", jmsFunc44Table2);

