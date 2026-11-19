#include "common.h"
#include "kernel.h"
#include "dds3KernelCore.h"
#include "code_r1_001CC1D0.h"
#include "fclMisc.h"

INCLUDE_ASM(const s32, "code_00344F78", DLCDestroy);

INCLUDE_ASM(const s32, "code_00344F78", DLCCalcProcess);

INCLUDE_ASM(const s32, "code_00344F78", fldCallGetProcessData4);

dds3ProcessFunc_t fldCallGetProcessData5(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    fclUpdateMessage();
    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00344F78", fldCallEndMessage);

INCLUDE_ASM(const s32, "code_00344F78", fldLeaf37);

INCLUDE_ASM(const s32, "code_00344F78", fldCallGetProcessData);

dds3ProcessFunc_t fldCallGetProcessData6(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t fldCallGetProcessData7(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00344F78", fldCommand_FLD_CALL_WAPEX);

INCLUDE_ASM(const s32, "code_00344F78", fldLeaf);

INCLUDE_ASM(const s32, "code_00344F78", fldCallGetProcessData8);

dds3ProcessFunc_t fldCallGetProcessData9(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t evtCallGetProcessData34(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    fclUpdateMessage();
    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00344F78", evtCallSetMessageInvalid);

/* body: matched in raidou @ 1424963: src/raidou1/code_002F5BD0.c:214, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s32 evtLeaf39(dds3ProcessID_t *PID) {
    return 1;
}

INCLUDE_ASM(const s32, "code_00344F78", evtPollDispatchAfterFade);

dds3ProcessFunc_t evtCallGetProcessData35(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t evtCallGetProcessData36(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    fclUpdateMessage();
    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00344F78", evtCallEndMessage);

INCLUDE_ASM(const s32, "code_00344F78", evtLeaf40);

INCLUDE_ASM(const s32, "code_00344F78", evtCallGetProcessData);

INCLUDE_ASM(const s32, "code_00344F78", evtBDispatchSyncD2Sub);

INCLUDE_ASM(const s32, "code_00344F78", evtBDispatchSyncD2);

INCLUDE_ASM(const s32, "code_00344F78", evtCallGetProcessData2);

INCLUDE_ASM(const s32, "code_00344F78", evtCallEndMessage2);

INCLUDE_ASM(const s32, "code_00344F78", gohFunc18);

INCLUDE_ASM(const s32, "code_00344F78", gohCheckDevilError);

INCLUDE_ASM(const s32, "code_00344F78", gohStatCalcProcess);

dds3ProcessFunc_t SetConfigData(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t GetConfigTab(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00344F78", InitMessage);

INCLUDE_ASM(const s32, "code_00344F78", evtLeaf41);

INCLUDE_ASM(const s32, "code_00344F78", evtBChooseSelectionCompletionPopup);

dds3ProcessFunc_t evtCallGetProcessData37(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t evtCallGetProcessData38(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    fclUpdateMessage();
    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00344F78", ChangeMessage);

INCLUDE_ASM(const s32, "code_00344F78", slLeaf12);

INCLUDE_ASM(const s32, "code_00344F78", slCallGetProcessData16);

dds3ProcessFunc_t slCallGetProcessData17(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t slInfoMsgDraw(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    fclUpdateMessage();
    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00344F78", slCallGetProcessData);

INCLUDE_ASM(const s32, "code_00344F78", slLeaf13);
