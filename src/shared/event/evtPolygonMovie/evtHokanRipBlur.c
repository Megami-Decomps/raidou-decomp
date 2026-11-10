#include "shared/event/evtPolygonMovie/decls.h"

void evtHokanRipBlur(s32 hokan, f32 ratio, dds3Effect_RippleBlurParam_t *key1, dds3Effect_RippleBlurParam_t *key2, dds3Effect_RippleBlurParam_t *d)
{
    s32 positionAxisIndex;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        key1 = &evtRipBlurDefaultParam;
    }
    if (key2 == NULL) {
        key2 = &evtRipBlurDefaultParam;
    }
    d->number = evtHokanUInt(hokan, ratio, key1->number, key2->number);
    d->speed = evtHokanFloat(hokan, ratio, key1->speed, key2->speed);
    d->interval = evtHokanFloat(hokan, ratio, key1->interval, key2->interval);
    d->abgr = evtHokanColor(hokan, ratio, key1->abgr, key2->abgr);
    d->blend = key1->blend;
    d->rotate = evtHokanFloat(hokan, ratio, key1->rotate, key2->rotate);
    d->power = evtHokanFloat(hokan, ratio, key1->power, key2->power);
    d->frequency = evtHokanFloat(hokan, ratio, key1->frequency, key2->frequency);
    for (positionAxisIndex = 0; positionAxisIndex < 2; positionAxisIndex++) {
        d->center[positionAxisIndex] = evtHokanInt(hokan, ratio, key1->center[positionAxisIndex], key2->center[positionAxisIndex]);
    }
    d->area = evtHokanUInt(hokan, ratio, key1->area, key2->area);
}
