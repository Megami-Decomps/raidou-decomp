#include "shared/sdf/sdfMotion/decls.h"

void sdfMotGetKey(sdfMotionNode_t *aThis, f32 aFrame, sdfMotGetKeyIf_t *aItf)
{
    sdfMotionCutChunk_t *chunk;
    u16 *frameList;
    u8 *keyData;
    u8 *key0;
    s32 frame;
    s32 numOfKey;
    s32 elemSize;
    s32 lo;
    s32 hi;
    s32 index;
    s32 next;
    s32 frame0;
    s32 frame1;
    s32 span;

    frame = aFrame;
    chunk = aThis->table;
    numOfKey = chunk->numOfKey;
    frameList = chunk->keyFrameList;
    elemSize = chunk->keyElemSize;
    lo = 0;
    hi = numOfKey - 1;
    do {
        index = (lo + hi + 1) >> 1;
        frame0 = frameList[index];
        if (frame < frame0) {
            index--;
            hi = index;
        } else {
            lo = index;
        }
    } while (lo < hi);
    keyData = (u8 *)&frameList[(numOfKey + 1) & ~1];
    frame0 = frameList[index];
    key0 = keyData + index * elemSize;
    aItf->key0 = key0;
    next = index + 1;
    if (next == numOfKey) {
        if (aThis->owner->playType == 0) {
            aItf->key1 = key0;
            frame1 = frame0;
        } else {
            aItf->key1 = keyData;
            frame1 = aThis->owner->frameLength;
        }
    } else {
        aItf->key1 = key0 + elemSize;
        frame1 = frameList[next];
    }
    span = frame1 - frame0;
    if (span == 0) {
        aItf->key1 = key0;
        aItf->rate = 0.0f;
        return;
    }
    aItf->rate = (aFrame - frame0) / span;
}
