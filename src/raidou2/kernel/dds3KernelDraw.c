#include "common.h"

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00105430);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3InitRenderDrawEnv);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001055F8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001058E0);

#include "shared/kernel/dds3KernelDraw/dds3AllocRenderTexture.c"

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3FreeRenderTexture);

#include "shared/kernel/dds3KernelDraw/dds3RenderTextureON.c"

#include "shared/kernel/dds3KernelDraw/dds3RenderTextureOFF.c"

#include "shared/kernel/dds3KernelDraw/dds3CheckRenderTexture.c"

#include "shared/kernel/dds3KernelDraw/dds3GetRenderTexture.c"

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00105D28);

#include "shared/kernel/dds3KernelDraw/dds3SetQuake.c"

#include "shared/kernel/dds3KernelDraw/dds3StopQuake.c"

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00105F88);

#include "shared/kernel/dds3KernelDraw/dds3InitZClearDrawTag.c"

#include "shared/kernel/dds3KernelDraw/dds3LinkZClearDrawTag.c"

#include "shared/kernel/dds3KernelDraw/dds3FadeClear.c"

#include "shared/kernel/dds3KernelDraw/dds3FadeSet.c"

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001061C0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001061D0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001061E8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001061F0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001061F8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106200);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106270);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001062D8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106340);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001063B0);

#include "shared/kernel/dds3KernelDraw/dds3FadeCheck.c"

#include "shared/kernel/dds3KernelDraw/dds3FadeUpdate.c"

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", dds3DrawDistRipOverError);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001065A8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106898);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001068E0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106978);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106A20);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106A38);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106AB8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106AC8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106B20);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00106C28);

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

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00109548);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00109660);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00109778);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001098B0);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00109950);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_001099E8);

INCLUDE_ASM(const s32, "kernel/dds3KernelDraw", func_00109A80);
INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE334);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE338);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE33C);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE340);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE344);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE348);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE34A);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE34C);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE350);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE354);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE356);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE358);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE360);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE364);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE368);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE36A);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE36C);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE36E);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE374);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE378);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE37B);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE37C);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE37D);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE380);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE383);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE384);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE386);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE388);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE38A);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE38C);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE390);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE394);

INCLUDE_SDATA(const s32, "kernel/dds3KernelDraw", D_004DE398);

