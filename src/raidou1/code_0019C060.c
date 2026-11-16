#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "event.h"
#include "scr.h"
#include "dds3SceneBasic.h"
#include "dds3KernelCore.h"
#include "mdlEffect.h"

s32 mdlPibGetVersion(pibHeaderChunk_t *record) {
    return record->payload.word;
}

u16 mdlPibGetChunkCount(pibHeaderChunk_t *record) {
    return record->parameter.part.count;
}
