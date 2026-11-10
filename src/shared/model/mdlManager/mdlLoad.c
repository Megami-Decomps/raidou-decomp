#include "shared/model/mdlManager/decls.h"

mdlObj_t *mdlLoad(u32 aMajor, u32 aMinor)
{
    dds3ModelResrc_t *node;
    mdlObj_t *ctx;
    mdlObj_t *head;
    sdfModelRootNode_t *inner;
    f32 modelScale;
    sdfVector4_t scale;
    s32 searchId;

    node = mdlLoadPrepare(aMajor, aMinor);
    ctx = sdfChipAllocZeroDirect(sizeof(mdlObj_t));
    ctx->sub = node;
    head = node->models;
    ctx->next = head;
    if (head != NULL)
    {
        head->prev = ctx;
    }
    node->models = ctx;
    inner = sdfModelMake(sdfTexListDup(node->texList), node->modelDefPtr);
    modelScale = node->scale;
    ctx->inner = inner;
    scale.x = modelScale;
    scale.y = modelScale;
    scale.z = modelScale;
    scale.w = 1.0f;
    SDF_VU0_COPYVECTOR(&inner->scale, &scale);
    ctx->objectRadius = node->radius;
    ctx->current.h.id = -1;
    ctx->current.h.motionIndex = -1;
    for (searchId = 0; searchId != MDL_SLOT_COUNT; searchId++)
    {
        if (node->playbooks[searchId].ptr != NULL && (node->playbooks[searchId].flags & 1))
        {
            mdlCreateMotion(ctx, searchId);
            if (searchId > 0)
            {
                if (node->playbooks[searchId].firstMotion >= 0)
                {
                    mdlLoopMotion(ctx, searchId, node->playbooks[searchId].firstMotion);
                }
            }
        }
    }
    ctx->resourceItems = NULL;
    mdlGenEffect(ctx, -1, -1);
    return ctx;
}
