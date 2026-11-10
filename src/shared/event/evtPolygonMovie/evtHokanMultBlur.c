#include "shared/event/evtPolygonMovie/decls.h"

void evtHokanMultBlur(s32 hokan, f32 ratio, dds3Effect_PartialMultiBlurParam_t *key1, dds3Effect_PartialMultiBlurParam_t *key2, dds3Effect_PartialMultiBlurParam_t *d)
{
    s32 positionOrCornerIndex;
    s32 coordinateIndex;

    if (hokan == 0) {
        ratio = 0.0f;
    }
    if (key1 == NULL) {
        key1 = &evtMultBlurDefaultParam;
    }
    if (key2 == NULL) {
        key2 = &evtMultBlurDefaultParam;
    }
    d->area = evtHokanUInt(hokan, ratio, key1->area, key2->area);
    d->Blur.abgr = evtHokanColor(hokan, ratio, key1->Blur.abgr, key2->Blur.abgr);
    d->Blur.blend = key1->Blur.blend;
    d->Blur.rotate = evtHokanFloat(hokan, ratio, key1->Blur.rotate, key2->Blur.rotate);
    d->Blur.scale = evtHokanFloat(hokan, ratio, key1->Blur.scale, key2->Blur.scale);
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        d->Blur.center[positionOrCornerIndex] = evtHokanInt(hokan, ratio, key1->Blur.center[positionOrCornerIndex], key2->Blur.center[positionOrCornerIndex]);
    }
    for (positionOrCornerIndex = 0; positionOrCornerIndex < 2; positionOrCornerIndex++) {
        for (coordinateIndex = 0; coordinateIndex < 2; coordinateIndex++) {
            d->Blur.xy[positionOrCornerIndex][coordinateIndex] = evtHokanInt(hokan, ratio, key1->Blur.xy[positionOrCornerIndex][coordinateIndex], key2->Blur.xy[positionOrCornerIndex][coordinateIndex]);
        }
    }
}
