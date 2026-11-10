#include "common.h"

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00105400);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3InitRenderDrawEnv);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001055C8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001058B0);

#include "shared/kernel/dds3KernelDraw/dds3AllocRenderTexture.c"

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3FreeRenderTexture);

#include "shared/kernel/dds3KernelDraw/dds3RenderTextureON.c"

#include "shared/kernel/dds3KernelDraw/dds3RenderTextureOFF.c"

#include "shared/kernel/dds3KernelDraw/dds3CheckRenderTexture.c"

#include "shared/kernel/dds3KernelDraw/dds3GetRenderTexture.c"

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00105CF8);

#include "shared/kernel/dds3KernelDraw/dds3SetQuake.c"

#include "shared/kernel/dds3KernelDraw/dds3StopQuake.c"

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00105F58);

#include "shared/kernel/dds3KernelDraw/dds3InitZClearDrawTag.c"

#include "shared/kernel/dds3KernelDraw/dds3LinkZClearDrawTag.c"

#include "shared/kernel/dds3KernelDraw/dds3FadeClear.c"

#include "shared/kernel/dds3KernelDraw/dds3FadeSet.c"

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106190);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001061A0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001061B8);

#include "shared/kernel/dds3KernelDraw/dds3FadeIn.c"

#include "shared/kernel/dds3KernelDraw/dds3FadeIn2.c"

#include "shared/kernel/dds3KernelDraw/dds3FadeOut.c"

#include "shared/kernel/dds3KernelDraw/dds3FadeOut2.c"

#include "shared/kernel/dds3KernelDraw/dds3FadeCheck.c"

#include "shared/kernel/dds3KernelDraw/dds3FadeUpdate.c"

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3DrawDistRipOverError);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001064B8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001067A8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001067E8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106880);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106928);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106940);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001069C0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106A18);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106B20);

#include "shared/kernel/dds3KernelDraw/dds3SetScreenFovy.c"

#include "shared/kernel/dds3KernelDraw/dds3SetAmbient.c"

#include "shared/kernel/dds3KernelDraw/dds3SetFog.c"

#include "shared/kernel/dds3KernelDraw/dds3SetLight.c"

#include "shared/kernel/dds3KernelDraw/setdraw_xy.c"

#include "shared/kernel/dds3KernelDraw/dds3SetFilter2Para.c"

#include "shared/kernel/dds3KernelDraw/dds3SetFilter2RGBA.c"

#include "shared/kernel/dds3KernelDraw/dds3Filter2Swap.c"

#include "shared/kernel/dds3KernelDraw/dds3Filter2On.c"

#include "shared/kernel/dds3KernelDraw/dds3Filter2Off.c"

#include "shared/kernel/dds3KernelDraw/dds3SetMFilterPara.c"

#include "shared/kernel/dds3KernelDraw/dds3SetMFilterRGBA.c"

#include "shared/kernel/dds3KernelDraw/dds3SetMFilterArea.c"

#include "shared/kernel/dds3KernelDraw/dds3MFilterSwap.c"

#include "shared/kernel/dds3KernelDraw/dds3MFilterOn.c"

#include "shared/kernel/dds3KernelDraw/dds3MFilterOff.c"

#include "shared/kernel/dds3KernelDraw/dds3SetBlur.c"

#include "shared/kernel/dds3KernelDraw/dds3SetBlur2Para.c"

#include "shared/kernel/dds3KernelDraw/dds3SetBlur2RGBA.c"

#include "shared/kernel/dds3KernelDraw/dds3SetBlur2Area.c"

#include "shared/kernel/dds3KernelDraw/dds3Blur2Swap.c"

#include "shared/kernel/dds3KernelDraw/dds3Blur2On.c"

#include "shared/kernel/dds3KernelDraw/dds3Blur2Off.c"

#include "shared/kernel/dds3KernelDraw/dds3SetMultiBlurPara.c"

#include "shared/kernel/dds3KernelDraw/dds3SetMultiBlurRGBA.c"

#include "shared/kernel/dds3KernelDraw/dds3SetMultiBlurArea.c"

#include "shared/kernel/dds3KernelDraw/dds3MultiBlurSwap.c"

#include "shared/kernel/dds3KernelDraw/dds3MultiBlurOn.c"

#include "shared/kernel/dds3KernelDraw/dds3MultiBlurOff.c"

#include "shared/kernel/dds3KernelDraw/dds3SetDistBlurPara.c"

#include "shared/kernel/dds3KernelDraw/dds3SetDistBlurRGBA.c"

#include "shared/kernel/dds3KernelDraw/dds3SetDistBlurArea.c"

#include "shared/kernel/dds3KernelDraw/dds3DistBlurSwap.c"

#include "shared/kernel/dds3KernelDraw/dds3DistBlurOn.c"

#include "shared/kernel/dds3KernelDraw/dds3DistBlurOff.c"

#include "shared/kernel/dds3KernelDraw/dds3SetRipBlurPara.c"

#include "shared/kernel/dds3KernelDraw/dds3SetRipBlurRGBA.c"

#include "shared/kernel/dds3KernelDraw/dds3SetRipBlurArea.c"

#include "shared/kernel/dds3KernelDraw/dds3RipBlurSwap.c"

#include "shared/kernel/dds3KernelDraw/dds3RipBlurOn.c"

#include "shared/kernel/dds3KernelDraw/dds3RipBlurOff.c"

#include "shared/kernel/dds3KernelDraw/dds3BlurUpdate.c"

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3RotationArc);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetLightColor);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SetLightDir);

#include "shared/kernel/dds3KernelDraw/dds3SetAmbientColor.c"

#include "shared/kernel/dds3KernelDraw/dds3SetFogColor.c"

#include "shared/kernel/dds3KernelDraw/dds3SetFogPara.c"

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3SceneLightUpdateFirst);

#include "shared/kernel/dds3KernelDraw/dds3SetSoftenBias.c"

#include "shared/kernel/dds3KernelDraw/eglInit.c"

#include "shared/kernel/dds3KernelDraw/eglSetOt.c"

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001093D0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001094E8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00109600);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00109738);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001097D8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00109870);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00109908);
INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B238);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B23C);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B240);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B244);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B248);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B24C);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B24E);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B250);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B254);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B258);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B25A);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B25C);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B260);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B264);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B268);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B26A);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B26C);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B26E);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B274);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B278);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B27B);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B27C);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B280);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B283);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B284);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B286);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B288);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B28A);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B28C);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B290);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_0046B294);

