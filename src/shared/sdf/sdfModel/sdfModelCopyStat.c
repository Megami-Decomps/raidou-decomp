#include "shared/sdf/sdfModel/decls.h"

/* vu0 routine: native SDK vector/matrix operations around model state. */
void sdfModelCopyStat(sdfModelRootNode_t *aDstRoot, sdfModelRootNode_t *aSrcRoot)
{
    s32 frame;
    s32 count;
    sdfModelNode_t **dstItems;
    sdfModelNode_t **srcItems;
    s32 i;

    sdfMatListCopyStat(aDstRoot->matList, aSrcRoot->matList);
    sdfModelMorphListCopyStat(aDstRoot->morphList, aSrcRoot->morphList);
    aDstRoot->flags = aSrcRoot->flags;
    aDstRoot->drawAttr = aSrcRoot->drawAttr;
    aDstRoot->color = aSrcRoot->color;
    SDF_VU0_LOADMATRIX(&aSrcRoot->tm);
    SDF_VU0_STOREMATRIX(&aDstRoot->tm);
    SDF_VU0_COPYVECTOR(&aDstRoot->rotate, &aSrcRoot->rotate);
    SDF_VU0_COPYVECTOR(&aDstRoot->scale, &aSrcRoot->scale);
    frame = (s8)sdfFrameId;
    *(u32 *)&aDstRoot->light = *(u32 *)&aSrcRoot->light;
    aDstRoot->faceDivSoften = aSrcRoot->faceDivSoften;
    *(u32 *)&aDstRoot->fog = *(u32 *)&aSrcRoot->fog;
    aDstRoot->faceDivBias = aSrcRoot->faceDivBias;
    count = aDstRoot->nodeList->count;
    dstItems = aDstRoot->nodeList->items;
    srcItems = aSrcRoot->nodeList->items;
    for (i = 0; i < count; i++) {
        sdfModelNode_t *src = srcItems[i];
        sdfModelNode_t *dst = dstItems[i];

        dst->flags = src->flags;
        dst->color = src->color;
        dst->drawAttr = src->drawAttr;
        SDF_VU0_COPYVECTOR(&dst->rotate, &src->rotate);
        SDF_VU0_COPYVECTOR(&dst->pos, &src->pos);
        SDF_VU0_COPYVECTOR(&dst->scale, &src->scale);
        SDF_VU0_LOADMATRIX(&src->localNoScaledTm);
        SDF_VU0_STOREMATRIX(&dst->localNoScaledTm);
        SDF_VU0_LOADMATRIX(&src->tm);
        SDF_VU0_STOREMATRIX(&dst->tm);
        if (dst->tmPacket != NULL) {
            *(sdfModelTmImage_t *)((u8 *)dst->tmPacket + (frame << 7)) =
                *(sdfModelTmImage_t *)((u8 *)src->tmPacket + (frame << 7));
        }
    }
}
