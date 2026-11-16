#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "event.h"
#include "scr.h"
#include "dds3SceneBasic.h"
#include "dds3KernelCore.h"
#include "sdf2D.h"

#define SDF_DMA_QWORD_SHIFT 4

u32 sdf2DGetTexPacketSize(sdfTexHandle_t *aTexHndl) {
    return 5 << SDF_DMA_QWORD_SHIFT;
}
