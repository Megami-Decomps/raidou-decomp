#include "common.h"
#include "dds3KernelDraw.h"
#include "mapFilter.h"

extern s32 mapCallFilterSetData[];
extern void fldSetFilterAnime(s32 anime);

void mapCallFilterSet(s32 no) {
    mapFilter_t *filter = &mapData33[no];
    f32 color[4];

    if (mapCallFilterSetData[0] != 0) {
        dds3FilterSet(filter->idx, filter->mode, 1);
        fldSetFilterAnime(filter->anime);
        dds3FilterSetAlphaOfs(filter->alphaOfs);
        dds3FilterSetAlpha(0x80);
        color[0] = filter->colorR * (1.0f / 256.0f);
        color[1] = filter->colorG * (1.0f / 256.0f);
        color[2] = filter->colorB * (1.0f / 256.0f);
        color[3] = 0;
        dds3SetFogColor(0, color);
        dds3SetFogPara(0, filter->fogFar, filter->fogMin, filter->fogNear, filter->fogMax);
    }
}
