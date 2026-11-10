#include "shared/effect/polyManager/decls.h"

void dds3SpPolySetLoop(dds3SpPolyLoopWork_t *hData, u8 loop) {
    if (hData->handler == 0) {
        hData->oldLoop = loop;
    }
    hData->loop = loop;
}
