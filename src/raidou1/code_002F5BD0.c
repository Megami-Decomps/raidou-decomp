#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "jmsMisc.h"
#include "TitleMain.h"
#include "fldProcess.h"
#include "sndAt3.h"
#include "mviManager.h"
#include "scrCommand.h"
#include "evtEventViewer.h"
#include "evtPolygonMovie.h"
#include "code_r1_002CAFD8.h"
#include "dds3PadManager.h"
#include "code_r1_001CC1D0.h"
#include "sdfMovie.h"
#include "dds3KernelDraw.h"
#include "rstResult.h"
#include "code_r1_002C7ED0.h"
#include "sdfSound.h"

extern jmsSeqUnit_t rstData2;
extern jmsSeqUnit_t TitleAtlusLogoCalcProcessData;
extern jmsSeqUnit_t evtBStartSelectionChainAfterFadeData;
extern jmsSeqUnit_t rstData;
extern jmsSeqUnit_t TitleConfigCalcProcessData;
extern jmsSeqUnit_t rstLvUpWaitCalcProcessData;

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

s32 evtCallPlay(s32 MovieNo, s32 Length, s32 FadeFrame, s32 Mode, titleData_t *pData) {
    s32 ret = 2;

    if (!(pData->flag & 0x20)) {
        if (pData->MoviePlayWait++ < 2) {
            return 0;
        }
        if (pData->MovieCounter != 0) {
            pData->MovieAlpha = 0;
        } else {
            pData->MovieAlpha = 128;
        }
        pData->MovieCounter = 0;
        ret = 0;
        sdfSetStreamFrameDrawMode(1);
        moviePlay(MovieNo);
        pData->MoviePlayWait = 0;
        pData->flag |= 0x20;
    } else {
        if (mviDrawMessageSub() == 0 || movieDecodeWork.frameIndex < 20) {
            return 0;
        }
        if (Length > 0) {
            Length -= 20;
            if (pData->MovieCounter >= Length - FadeFrame && pData->MovieAlpha > 0) {
                pData->MovieAlpha = ((Length - pData->MovieCounter) << 7) / FadeFrame;
            }
            if (pData->MovieCounter <= FadeFrame && pData->MovieAlpha < 128) {
                pData->MovieAlpha = (pData->MovieCounter << 7) / FadeFrame;
            }
            if (pData->MovieAlpha < 0) {
                pData->MovieAlpha = 0;
            }
            if (pData->MovieAlpha > 128) {
                pData->MovieAlpha = 128;
            }
        }
        if (mviValidateMovieSub() != 0 && pData->MovieCounter > 0) {
            evtCallStop(pData);
            switch (Mode) {
            case 0:
                ret = 3;
                break;
            case 1:
                ret = 2;
                break;
            }
        } else if (pData->flag & 0x40) {
            evtCallStop(pData);
            return 2;
        }
        if (pData->MovieCounter == 0) {
            ret = 1;
        }
        pData->MovieCounter++;
    }
    sdfSetGridScaledDrawBounds(0, 0, 512, 448, (pData->MovieAlpha << 24) | 0x808080);
    return ret;
}

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

void evtCallBgmStop(titleData_t *pData) {
    if (pData->flag & 8) {
        sndBgmStop();
        sndBgmFree();
    }
    pData->flag &= ~4;
    pData->flag &= ~8;
    pData->flag &= ~0x10;
}

s32 TitleFirstCalcProcess3(dds3ProcessID_t *PID) {
    titleData_t *data = dds3GetProcessData(PID);
    s32 ret;
    u32 i;
    s32 midi;

    ret = jmsSeqChange(&data->panel, &data->seqUnit, 0, PID);
    if (ret != 0) {
        return ret;
    }
    effPollResourceList(data->spriteList);
    for (i = 0; i < 4; i++) {
        if (data->sprite[i] == NULL) {
            return 0;
        }
    }
    data->flag |= 2;
    midi = p2sdLoadCheckMidi(0x500000);
    switch (midi) {
    case 0:
        p2sdLoadSMGFile(0x500000);
        break;
    case 2:
        break;
    default:
        if (data->PmvPID == NULL) {
            scrSetMovieCancel(0);
            data->PmvPID = evtStart2(DDS3_KERNELPROC_PRIORITY, 600, 1, 2);
            dds3LinkProcess(PID, data->PmvPID);
            evtNoSkipMes(data->PmvPID);
            evtIgnoreAtracPlay(data->PmvPID);
        }
        if (evtCheckRun(data->PmvPID) == 0) {
            return 0;
        }
        evtSetFrame(data->PmvPID, 90);
        switch (data->mode) {
        case 0:
            if (TitleAt3Load(data) == 0) {
                return 0;
            }
            jmsSeqJump(&data->seqUnit, &rstData);
            break;
        case 2:
            dds3FadeOut(0, 0, 0, 0);
            data->flag |= 0x1C;
            if (evtCallPlay(25, 5400, 30, 1, data) == 0) {
                return 0;
            }
            Flow1_6(&data->Cursor);
            dds3FadeIn(0, 0, 0, 30);
            jmsSeqJump(&data->seqUnit, &TitleConfigCalcProcessData);
            break;
        }
        break;
    }
    return 0;
}

s32 TitleFirstDrawProcess(dds3ProcessID_t *PID) {
    titleData_t *data = dds3GetProcessData(PID);
    s32 ret;

    ret = jmsSeqChange(&data->panel, &data->seqUnit, 1, PID);
    jmsSeqChange(&data->panel, &data->seqUnit, 2, PID);
    return ret;
}

s32 TitleCesaInit(dds3ProcessID_t *PID) {
    titleData_t *data = dds3GetProcessData(PID);

    data->WaitTimer = 0;
    dds3FadeIn(0, 0, 0, 40);
    return 1;
}

s32 evtLeaf32(dds3ProcessID_t *PID) {
    return 1;
}

s32 TitleAtlusLogoCalcProcess(dds3ProcessID_t *PID) {
    titleData_t *data = dds3GetProcessData(PID);
    s32 ret;
    s32 skip;

    ret = jmsSeqChange(&data->panel, &data->seqUnit, 0, PID);
    if (ret != 0) {
        return ret;
    }
    ret = TitleAt3Load(data);
    if (ret == 0) {
        return ret;
    }
    skip = FALSE;
    if (dds3PadButtonState[0x23] < 0) {
        skip = TRUE;
    }
    if (dds3PadButtonState[0x2C] < 0) {
        skip = TRUE;
    }
    if (skip && (u32)(data->WaitTimer - 40) < 60) {
        data->WaitTimer = 100;
    }
    if (data->WaitTimer == 100) {
        dds3FadeOut(0, 0, 0, 40);
    }
    if (data->WaitTimer >= 140) {
        jmsSeqJumpAndFree(&data->seqUnit, &TitleAtlusLogoCalcProcessData);
    }
    data->WaitTimer++;
    return 0;
}

s32 TitleAtlusLogoDrawProcess(dds3ProcessID_t *PID) {
    titleData_t *data = dds3GetProcessData(PID);
    s32 ret;

    itfDrawGridWithResolvedSlot(1648, 1352, 0, 0, data->sprite[0], 1, 103);
    ret = jmsSeqChange(&data->panel, &data->seqUnit, 1, PID);
    jmsSeqChange(&data->panel, &data->seqUnit, 2, PID);
    return ret;
}

s32 evtCallGetProcessData31(dds3ProcessID_t *PID) {
    titleData_t *data = dds3GetProcessData(PID);

    data->WaitTimer = 0;
    dds3FadeIn(0, 0, 0, 40);
    return 1;
}

s32 evtLeaf33(dds3ProcessID_t *PID) {
    return 1;
}

s32 evtBStartSelectionChainAfterFade(dds3ProcessID_t *PID) {
    titleData_t *data = dds3GetProcessData(PID);
    s32 ret;
    s32 skip;

    ret = jmsSeqChange(&data->panel, &data->seqUnit, 0, PID);
    if (ret != 0) {
        return ret;
    }
    skip = FALSE;
    if (dds3PadButtonState[0x23] < 0) {
        skip = TRUE;
    }
    if (dds3PadButtonState[0x2C] < 0) {
        skip = TRUE;
    }
    if (skip && (u32)(data->WaitTimer - 40) < 60) {
        data->WaitTimer = 100;
    }
    if (data->WaitTimer == 100) {
        dds3FadeOut(0, 0, 0, 40);
    }
    if (data->WaitTimer >= 140) {
        jmsSeqJumpAndFree(&data->seqUnit, &evtBStartSelectionChainAfterFadeData);
    }
    data->WaitTimer++;
    return 0;
}

s32 evtCallGetProcessData32(dds3ProcessID_t *PID) {
    titleData_t *data = dds3GetProcessData(PID);
    s32 ret;

    itfDrawGridWithResolvedSlot(816, 1160, 0, 0, data->sprite[0], 2, 103);
    ret = jmsSeqChange(&data->panel, &data->seqUnit, 1, PID);
    jmsSeqChange(&data->panel, &data->seqUnit, 2, PID);
    return ret;
}

void TitleGoMain(titleData_t *pData) {
    scrSetMovieCancel(0);
    evtChgSeq(pData->PmvPID, 2);
    evtNoSkipMes(pData->PmvPID);
    evtIgnoreAtracPlay(pData->PmvPID);
    pData->WaitTimer = 60;
    jmsSeqJumpAndFree(&pData->seqUnit, &rstData2);
}

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

s32 evtLeaf34(dds3ProcessID_t *PID) {
    return 1;
}

s32 evtCallGetProcessData33(dds3ProcessID_t *PID) {
    titleData_t *data = dds3GetProcessData(PID);
    s32 ret;
    s32 end;

    ret = jmsSeqChange(&data->panel, &data->seqUnit, 0, PID);
    if (ret != 0) {
        return ret;
    }
    end = FALSE;
    if (evtCheckSeqEnd(data->PmvPID) != 0) {
        end = TRUE;
    }
    if (mviDrawMessageSub() != 0 && movieDecodeWork.frameIndex >= 20
        && movieDecodeWork.frameIndex < movieDecodeWork.frameCount - 30 && data->MovieSkipFlag == 0) {
        if (dds3PadButtonState[0x23] < 0) {
            data->MovieSkipFlag = 1;
        }
        if (dds3PadButtonState[0x2C] < 0) {
            data->MovieSkipFlag = 1;
        }
    }
    switch (data->MovieSkipFlag) {
    case 1:
        dds3BistaIn(0);
        dds3FadeSet(255, 255, 255, (data->SkipTimer << 7) / 5);
        if (data->SkipTimer < 5) {
            data->SkipTimer++;
        } else {
            data->MovieSkipFlag = 2;
        }
        break;
    case 2:
        end = TRUE;
        dds3BistaIn(0);
        dds3FadeSet(255, 255, 255, 128);
        break;
    }
    if (end) {
        TitleGoMain(data);
    }
    return 0;
}

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

s32 rstLeaf10(dds3ProcessID_t *PID) {
    return 1;
}

s32 rstLvUpWaitCalcProcess(dds3ProcessID_t *PID) {
    titleData_t *data = dds3GetProcessData(PID);
    s32 ret;

    ret = jmsSeqChange(&data->panel, &data->seqUnit, 0, PID);
    if (ret != 0) {
        return ret;
    }
    if (evtCallPlay(25, 5400, 30, 1, data) == 0) {
        evtSetFrame(data->PmvPID, 0);
        return 0;
    }
    TitleAt3Play(data);
    if (data->WaitTimer > 0) {
        data->WaitTimer--;
        return 0;
    }
    if (data->Timer < 1200) {
        if (dds3PadButtonState[0x23] < 0 || dds3PadButtonState[0x2C] < 0) {
            Flow1_6(&data->Cursor);
            jmsSeqJumpAndFree(&data->seqUnit, &TitleConfigCalcProcessData);
            p2sdPlaySE(0x500000, 127, 63);
        }
    } else if (data->FadeOut == 0) {
        sndBgmFadeOut();
        dds3FadeOut(0, 0, 0, 60);
        data->FadeOut = 1;
    } else if (dds3FadeCheck() == 0) {
        if (evtCallStop(data) == 0) {
            return 0;
        }
        evtCallBgmStop(data);
        if (data->AdvSkipFlag == 0) {
            jmsSeqJumpAndFree(&data->seqUnit, &rstLvUpWaitCalcProcessData);
        } else {
            jmsSeqJumpAndFree(&data->seqUnit, &rstData);
            data->AdvSkipFlag = 0;
        }
    }
    data->Timer++;
    return 0;
}
