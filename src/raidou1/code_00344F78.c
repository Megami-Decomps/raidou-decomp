#include "common.h"
#include "kernel.h"
#include "dds3KernelCore.h"
#include "code_r1_001CC1D0.h"
#include "fclMisc.h"

INCLUDE_ASM(const s32, "code_00344F78", gohKarteRegAllDestroy);

INCLUDE_ASM(const s32, "code_00344F78", gohKarteRegAllCalcProcess);

INCLUDE_ASM(const s32, "code_00344F78", gohKarteRegAllDrawProcess);

dds3ProcessFunc_t gohKarteRegAllUpdateProcess(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    fclUpdateMessage();
    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00344F78", gohKarteRegStatInit);

INCLUDE_ASM(const s32, "code_00344F78", gohKarteRegStatDestroy);

INCLUDE_ASM(const s32, "code_00344F78", gohKarteRegStatCalcProcess);

dds3ProcessFunc_t gohKarteRegStatDrawProcess(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t gohKarteRegStatUpdateProcess(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00344F78", gohKarteRegConfirmInit);

INCLUDE_ASM(const s32, "code_00344F78", gohKarteRegConfirmDestroy);

INCLUDE_ASM(const s32, "code_00344F78", gohKarteRegConfirmCalcProcess);

dds3ProcessFunc_t gohKarteRegConfirmDrawProcess(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t gohKarteRegConfirmUpdateProcess(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    fclUpdateMessage();
    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00344F78", gohKarteRegResultInit);

/* body: matched in raidou @ 1424963: src/raidou1/code_002F5BD0.c:214, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s32 gohKarteRegResultDestroy(dds3ProcessID_t *PID) {
    return 1;
}

INCLUDE_ASM(const s32, "code_00344F78", gohKarteRegResultCalcProcess);

dds3ProcessFunc_t gohKarteRegResultDrawProcess(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t gohKarteRegResultUpdateProcess(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    fclUpdateMessage();
    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00344F78", gohKarteResInit);

INCLUDE_ASM(const s32, "code_00344F78", gohKarteResDestroy);

INCLUDE_ASM(const s32, "code_00344F78", gohKarteResCalcProcess);

INCLUDE_ASM(const s32, "code_00344F78", evtBDispatchSyncD2Sub);

INCLUDE_ASM(const s32, "code_00344F78", gohKarteResDrawProcess);

INCLUDE_ASM(const s32, "code_00344F78", gohKarteResUpdateProcess);

INCLUDE_ASM(const s32, "code_00344F78", gohKarteResStatInit);

INCLUDE_ASM(const s32, "code_00344F78", gohKarteResStatDestroy);

INCLUDE_ASM(const s32, "code_00344F78", gohCheckDevilError);

INCLUDE_ASM(const s32, "code_00344F78", gohKarteResStatCalcProcess);

dds3ProcessFunc_t gohKarteResStatDrawProcess(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t gohKarteResStatUpdateProcess(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00344F78", gohKarteResConfirmInit);

INCLUDE_ASM(const s32, "code_00344F78", gohKarteResConfirmDestroy);

INCLUDE_ASM(const s32, "code_00344F78", gohKarteResConfirmCalcProcess);

dds3ProcessFunc_t gohKarteResConfirmDrawProcess(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t gohKarteResConfirmUpdateProcess(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    fclUpdateMessage();
    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00344F78", gohKarteResResultInit);

INCLUDE_ASM(const s32, "code_00344F78", gohKarteResResultDestroy);

INCLUDE_ASM(const s32, "code_00344F78", gohKarteResResultCalcProcess);

dds3ProcessFunc_t gohKarteResResultDrawProcess(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t gohKarteResResultUpdateProcess(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    fclUpdateMessage();
    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00344F78", gohKarteErrorInit);

INCLUDE_ASM(const s32, "code_00344F78", gohKarteErrorDestroy);
