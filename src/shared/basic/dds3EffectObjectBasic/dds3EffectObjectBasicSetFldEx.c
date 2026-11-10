#include "shared/basic/dds3EffectObjectBasic/decls.h"

void dds3EffectObjectBasicSetFldEx(dds3Basic_t *basic, fldResourceEffect_t *pResEff) {
    dds3EffectObjectBasicWorkData_t *resource = basic->workdata;
    dds3PoseSet_t *source = basic->poseset;

    resource->pFldEx = pResEff;
    resource->FldExBasePos.x = source->pos.x;
    resource->FldExBasePos.y = source->pos.y;
    resource->FldExBasePos.z = source->pos.z;
    resource->FldExBasePos.w = source->pos.w;
    dds3EffectObjectBasicStatOn((struct dds3Basic_t *)basic, 0x10);
    switch (resource->pFldEx->type) {
    case 0:
    case 3:
        dds3BillboardSetXYOffset((struct billHandle_t *)resource->handle, resource->pFldEx->size[0], resource->pFldEx->size[1]);
        if (resource->pFldEx->type == 3) {
            dds3BillboardSetDrawType(resource->handle, 1);
        }
        if (resource->pFldEx->flag & 1) {
            dds3BillboardSetMode(resource->handle, 2);
            return;
        }
        if (resource->pFldEx->flag & 2) {
            dds3BillboardSetMode(resource->handle, 3);
            return;
        }
        break;
    case 2:
        break;
    case 1:
        dds3BillboardSetAnimNumber(resource->handle, resource->pFldEx->param0);
        break;
    }
}
