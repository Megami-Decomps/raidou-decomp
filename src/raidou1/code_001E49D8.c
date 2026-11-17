#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "evtTmxPreLoad.h"
#include "billboard.h"
#include "billManager.h"
#include "fileManager.h"
#include "sdfMemory.h"
#include "effPacket.h"
#include "dds3KernelDraw.h"
#include "sdk_libc.h"

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPacket.c:359, found, same function 1.00 (words 12/12, calls 1.00, shape 1.00) */
void dds3CrossfadeEnd(void) {
    dds3RenderTextureOFF();
    dds3FreeRenderTexture();
    dds3CrossfadeRunFlag = 0;
    printf("dds3CrossfadeEnd\n");
}
