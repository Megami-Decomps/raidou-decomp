#include "common.h"
#include "dds3KernelCore.h"
#include "TitleMain.h"
#include "sndAt3.h"
#include "mviManager.h"
#include "scrCommand.h"
#include "evtEventViewer.h"
#include "evtPolygonMovie.h"
#include "sdfMovie.h"

s32 evtCallStop(titleData_t *pData) {
    if (!(pData->flag & 0x40)) {
        if (pData->flag & 0x20) {
            movieStop();
            pData->flag |= 0x40;
            sdfSetStreamFrameDrawMode(0);
            return 0;
        }
    }
    pData->MovieCounter = 0;
    pData->flag &= ~0x20;
    pData->flag &= ~0x40;
    return 1;
}

INCLUDE_ASM(const s32, "code_0035B048", evtCallPlay);

s32 TitleAt3Load(titleData_t *pData) {
    if (!(pData->flag & 4)) {
        sndBgmLoadId_XF(80);
        pData->flag |= 4;
    }
    if (sndBgmStatus_XF() != 2) {
        return 0;
    }
    pData->flag |= 8;
    return 1;
}

void TitleAt3Play(titleData_t *pData) {
    u32 flag = pData->flag;

    if (flag & 8) {
        if (!(flag & 0x10)) {
            sndBgmPlay();
            pData->flag |= 0x10;
        }
    }
}

INCLUDE_ASM(const s32, "code_0035B048", evtCallBgmStop);

INCLUDE_ASM(const s32, "code_0035B048", TitleFirstCalcProcess3);

s32 TitleFirstDrawProcess(dds3ProcessID_t *PID) {
    titleData_t *data = dds3GetProcessData(PID);
    s32 ret;

    ret = jmsSeqChange(&data->panel, &data->seqUnit, 1, PID);
    jmsSeqChange(&data->panel, &data->seqUnit, 2, PID);
    return ret;
}

INCLUDE_ASM(const s32, "code_0035B048", TitleCesaInit);

INCLUDE_ASM(const s32, "code_0035B048", evtLeaf32);

INCLUDE_ASM(const s32, "code_0035B048", TitleAtlusLogoCalcProcess);

INCLUDE_ASM(const s32, "code_0035B048", TitleAtlusLogoDrawProcess);

INCLUDE_ASM(const s32, "code_0035B048", evtCallGetProcessData31);

INCLUDE_ASM(const s32, "code_0035B048", evtLeaf33);

INCLUDE_ASM(const s32, "code_0035B048", evtBStartSelectionChainAfterFade);

INCLUDE_ASM(const s32, "code_0035B048", evtCallGetProcessData32);

INCLUDE_ASM(const s32, "code_0035B048", TitleGoMain);

s32 TitleMovieInit(dds3ProcessID_t *PID) {
    titleData_t *data = dds3GetProcessData(PID);

    data->SkipTimer = 0;
    data->MovieSkipFlag = 0;
    if (data->PmvPID == NULL) {
        scrSetMovieCancel(0);
        data->PmvPID = evtStart2(DDS3_KERNELPROC_PRIORITY, 600, 1, 1);
        dds3LinkProcess(PID, data->PmvPID);
        evtNoSkipMes(data->PmvPID);
        evtIgnoreAtracPlay(data->PmvPID);
    } else {
        scrSetMovieCancel(0);
        evtChgSeq(data->PmvPID, 1);
        evtNoSkipMes(data->PmvPID);
        evtIgnoreAtracPlay(data->PmvPID);
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_0035B048", evtLeaf34);

INCLUDE_ASM(const s32, "code_0035B048", evtCallGetProcessData33);

s32 rstCallGetProcessData28(dds3ProcessID_t *PID) {
    titleData_t *data = dds3GetProcessData(PID);
    s32 ret;

    ret = jmsSeqChange(&data->panel, &data->seqUnit, 1, PID);
    jmsSeqChange(&data->panel, &data->seqUnit, 2, PID);
    return ret;
}

void rstFunc(titleData_t *data) {
    evtFunc10(4, 256, data->sprite);
    evtFunc10(2, 256, data->sprite);
    evtFunc10(3, 256, data->sprite);
}

s32 rstCallGetProcessData29(dds3ProcessID_t *PID) {
    titleData_t *data = dds3GetProcessData(PID);

    data->Timer = 0;
    data->introFrame = 0;
    data->FadeOut = 0;
    return 1;
}

INCLUDE_ASM(const s32, "code_0035B048", rstLeaf10);

INCLUDE_ASM(const s32, "code_0035B048", rstLvUpWaitCalcProcess);
