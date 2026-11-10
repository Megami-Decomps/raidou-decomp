#include "shared/event/evtPolygonMovie/decls.h"

void evtHokanFilter(s32 hokan, f32 ratio, dds3Effect_Filter_t *key1, dds3Effect_Filter_t *key2, dds3Effect_Filter_t *d)
{
    s32 cornerIndex;
    s32 coordinateIndex;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        key1 = &evtFilterDefaultParam;
    }
    if (key2 == NULL) {
        key2 = &evtFilterDefaultParam;
    }
    for (cornerIndex = 0; cornerIndex < 2; cornerIndex++) {
        for (coordinateIndex = 0; coordinateIndex < 2; coordinateIndex++) {
            d->xy[cornerIndex][coordinateIndex] = evtHokanInt(hokan, ratio, key1->xy[cornerIndex][coordinateIndex], key2->xy[cornerIndex][coordinateIndex]);
        }
    }
    d->abgr = evtHokanColor(hokan, ratio, key1->abgr, key2->abgr);
    d->blend = key1->blend;
}
