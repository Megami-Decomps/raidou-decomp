#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "evtEventViewer.h"
#include "evtEventViewerMenu.h"
#include "sdfChip.h"
#include "code_r1_001C4B78.h"

void evtLeaf14(evtViewerInfo_t *scene, s32 delta) {
    evtViewerObject_t *track = scene->framelist_target;
    evtFrameInfo_t *key;
    s32 absoluteFrame;

    if (track == NULL) {
        return;
    }
    for (key = track->children; key != NULL; key = key->next) {
        absoluteFrame = key->frame + track->metadata.value + delta;
        if (absoluteFrame < track->metadata.value) {
            key->frame = 0;
        } else if (scene->totalframe < absoluteFrame) {
            key->frame = (u16)scene->totalframe - (u16)track->metadata.value - 1;
        } else {
            key->frame = key->frame + delta;
        }
    }
}
