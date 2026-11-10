#include "shared/event/evtPolygonMovie/decls.h"

void evtHokanMultFilter(s32 hokan, f32 ratio, dds3Effect_PartialMultiFilterParam_t *key1, dds3Effect_PartialMultiFilterParam_t *key2, dds3Effect_PartialMultiFilterParam_t *d)
{
    s32 centerCoordinateIndex;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        key1 = &evtMultFilterDefaultParam;
    }
    if (key2 == NULL) {
        key2 = &evtMultFilterDefaultParam;
    }
    d->area = evtHokanUInt(hokan, ratio, key1->area, key2->area);
    for (centerCoordinateIndex = 0; centerCoordinateIndex < 2; centerCoordinateIndex++) {
        d->center[centerCoordinateIndex] = evtHokanInt(
            hokan, ratio, key1->center[centerCoordinateIndex], key2->center[centerCoordinateIndex]);
    }
    d->Filter.rgba = evtHokanColor(hokan, ratio, key1->Filter.rgba, key2->Filter.rgba);
    d->Filter.blendControl = key1->Filter.blendControl;
}
