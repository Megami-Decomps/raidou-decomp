#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3BlurUpdate(void)
{
    s32 alpha;

    if (dds3DrawFlag & 0x800) {
        blurnow++;
        dds3ScreenBlurAlpha = bluralphabase + blurnow * (bluralphatarget - bluralphabase) / blurmax;
        dds3ScreenBlurScale = blurscalebase + blurnow * (blurscaletarget - blurscalebase) / blurmax;
        if (dds3ScreenBlurAlpha == 0 && dds3ScreenBlurScale == 0) {
            dds3ScreenBlurFlag = 0;
        }
        else {
            dds3ScreenBlurFlag = 1;
        }
        if (blurnow >= blurmax) {
            dds3DrawFlag &= 0xfffff7ff;
        }
    }
    if (dds3DrawFlag & 0x40000) {
        if (blur2swapnow < blur2swapmax) {
            blur2swapnow++;
        }
        evtHokanBlur2(1, (f32)blur2swapnow / (f32)blur2swapmax, &ripblurtarget.blur2buf,
                        &blur2target, &dds3Blur2State);
        setdraw_xy(dds3Blur2State.rect);
        Eve_BlurSetParam(&dds3Blur2State);
        if (dds3Blur2State.rgba[3] != 0) {
            Eve_BlurON();
        }
        else {
            Eve_BlurOFF();
        }
        if (blur2swapnow >= blur2swapmax) {
            dds3DrawFlag &= ~0x40000;
        }
    }
    if (dds3DrawFlag & 0x20000) {
        if (blur2now < blur2max) {
            blur2now++;
        }
        dds3Blur2State = blur2target;
        alpha = blur2abase + blur2now *
                (blur2atarget - blur2abase) /
                blur2max;
        dds3Blur2State.color = (dds3Blur2State.color & 0xFFFFFF) | (alpha << 24);
        setdraw_xy(dds3Blur2State.rect);
        Eve_BlurSetParam(&dds3Blur2State);
        if (alpha == 0) {
            Eve_BlurOFF();
        }
        else {
            Eve_BlurON();
        }
        if (blur2now >= blur2max) {
            dds3DrawFlag &= ~0x20000;
        }
    }
    if (dds3DrawFlag & 0x8000) {
        if (mulblurswapnow < mulblurswapmax) {
            mulblurswapnow++;
        }
        evtHokanMultBlur(1, (f32)mulblurswapnow / (f32)mulblurswapmax, &dds3MultiBlurBuf,
                        &mulblurtarget, &key2);
        Eve_PartialMultiBlurSetParam(&key2);
        if (key2.rgba[3] != 0) {
            Eve_PartialMultiBlurON();
        }
        else {
            Eve_PartialMultiBlurOFF();
        }
        if (mulblurswapnow >= mulblurswapmax) {
            dds3DrawFlag &= ~0x8000;
        }
    }
    if (dds3DrawFlag & 0x1000) {
        if (mulblurnow < mulblurmax) {
            mulblurnow++;
        }
        key2 = mulblurtarget;
        alpha = mulblurabase + mulblurnow *
                (mulbluratarget - mulblurabase) /
                mulblurmax;
        key2.color = (key2.color & 0xFFFFFF) | (alpha << 24);
        Eve_PartialMultiBlurSetParam(&key2);
        if (alpha == 0) {
            Eve_PartialMultiBlurOFF();
        }
        else {
            Eve_PartialMultiBlurON();
        }
        if (mulblurnow >= mulblurmax) {
            dds3DrawFlag &= ~0x1000;
        }
    }
    if (dds3DrawFlag & 0x10000) {
        if (distblurswapnow < distblurswapmax) {
            distblurswapnow++;
        }
        evtHokanDistBlur(1, (f32)distblurswapnow / (f32)distblurswapmax, &dds3DistBlurBuf,
                        &distblurtarget, &dds3DistBlurCurrent);
        Eve_DistortionBlurSetParam(&dds3DistBlurCurrent);
        if (dds3DistBlurCurrent.rgba[3] != 0) {
            Eve_DistortionBlurON();
        }
        else {
            Eve_DistortionBlurOFF();
        }
        if (distblurswapnow >= distblurswapmax) {
            dds3DrawFlag &= ~0x10000;
        }
    }
    if (dds3DrawFlag & 0x2000) {
        if (distblurnow < distblurmax) {
            distblurnow++;
        }
        dds3DistBlurCurrent = distblurtarget;
        alpha = distblurabase + distblurnow *
                (distbluratarget - distblurabase) /
                distblurmax;
        dds3DistBlurCurrent.color = (dds3DistBlurCurrent.color & 0xFFFFFF) | (alpha << 24);
        Eve_DistortionBlurSetParam(&dds3DistBlurCurrent);
        if (alpha == 0) {
            Eve_DistortionBlurOFF();
        }
        else {
            Eve_DistortionBlurON();
        }
        if (distblurnow >= distblurmax) {
            dds3DrawFlag &= ~0x2000;
        }
    }
    if (dds3DrawFlag & 0x1000000) {
        if (ripblurswapnow < ripblurswapmax) {
            ripblurswapnow++;
        }
        evtHokanRipBlur(1, (f32)ripblurswapnow / (f32)ripblurswapmax, &dds3RipBlurBuf,
                        &ripblurtarget.ripple, &dds3BlurState.ripple);
        Eve_RippleBlurSetParam(&dds3BlurState.ripple);
        if (dds3BlurState.ripple.rgba[3] != 0) {
            Eve_RippleBlurON();
        }
        else {
            Eve_RippleBlurOFF();
        }
        if (ripblurswapnow >= ripblurswapmax) {
            dds3DrawFlag &= ~0x1000000;
        }
    }
    if (dds3DrawFlag & 0x800000) {
        if (ripblurnow < ripblurmax) {
            ripblurnow++;
        }
        dds3BlurState.ripple = ripblurtarget.ripple;
        alpha = ripblurabase + ripblurnow *
                (ripbluratarget - ripblurabase) /
                ripblurmax;
        dds3BlurState.ripple.color = (dds3BlurState.ripple.color & 0xFFFFFF) | (alpha << 24);
        Eve_RippleBlurSetParam(&dds3BlurState.ripple);
        if (alpha == 0) {
            Eve_RippleBlurOFF();
        }
        else {
            Eve_RippleBlurON();
        }
        if (ripblurnow >= ripblurmax) {
            dds3DrawFlag &= ~0x800000;
        }
    }
    if (dds3DrawFlag & 0x100000) {
        if (filter2swapnow < filter2swapmax) {
            filter2swapnow++;
        }
        evtHokanFilter(1, (f32)filter2swapnow / (f32)filter2swapmax, &dds3Filter2Buf,
                                        &filter2target, &dds3BlurParam);
        setdraw_xy(dds3BlurParam.rect);
        Eve_FilterSetParam(&dds3BlurParam);
        if (dds3BlurParam.rgba[3] != 0) {
            Eve_FilterON();
        }
        else {
            Eve_FilterOFF();
        }
        if (filter2swapnow >= filter2swapmax) {
            dds3DrawFlag &= ~0x100000;
        }
    }
    if (dds3DrawFlag & 0x80000) {
        if (filter2now < filter2max) {
            filter2now++;
        }
        dds3BlurParam = filter2target;
        alpha = filter2abase + filter2now *
                (filter2atarget - filter2abase) /
                filter2max;
        dds3BlurParam.color = (dds3BlurParam.color & 0xFFFFFF) | (alpha << 24);
        setdraw_xy(dds3BlurParam.rect);
        Eve_FilterSetParam(&dds3BlurParam);
        if (alpha == 0) {
            Eve_FilterOFF();
        }
        else {
            Eve_FilterON();
        }
        if (filter2now >= filter2max) {
            dds3DrawFlag &= ~0x80000;
        }
    }
    if (dds3DrawFlag & 0x400000) {
        if (mfilterswapnow < mfilterswapmax) {
            mfilterswapnow++;
        }
        evtHokanMultFilter(1, (f32)mfilterswapnow / (f32)mfilterswapmax, &dds3MFilterBuf,
                        &mfiltertarget, &dds3MFilterCurrent);
        Eve_PartialMultiFilterSetParam(&dds3MFilterCurrent);
        if (dds3MFilterCurrent.rgba[3] != 0) {
            Eve_PartialMultiFilterON();
        }
        else {
            Eve_PartialMultiFilterOFF();
        }
        if (mfilterswapnow >= mfilterswapmax) {
            dds3DrawFlag &= ~0x400000;
        }
    }
    if (dds3DrawFlag & 0x200000) {
        if (mfilternow < mfiltermax) {
            mfilternow++;
        }
        dds3MFilterCurrent = mfiltertarget;
        alpha = mfilterabase + mfilternow *
                (mfilteratarget - mfilterabase) /
                mfiltermax;
        dds3MFilterCurrent.color = (dds3MFilterCurrent.color & 0xFFFFFF) | (alpha << 24);
        Eve_PartialMultiFilterSetParam(&dds3MFilterCurrent);
        if (alpha == 0) {
            Eve_PartialMultiFilterOFF();
        }
        else {
            Eve_PartialMultiFilterON();
        }
        if (mfilternow >= mfiltermax) {
            dds3DrawFlag &= ~0x200000;
        }
    }
}
