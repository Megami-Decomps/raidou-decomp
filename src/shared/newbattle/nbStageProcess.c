#include "common.h"
#include "basic.h"
#include "field.h"
#include "dds3obj.h"
#include "dds3Basic.h"
#include "dds3PathBasic.h"
#include "dds3EffectObjectBasic.h"
#include "dds3LightObjectBasic.h"
#include "fldFileResolver_00132080.h"

extern fldResHead_t *fldScriptResource;
extern u32 fldScriptResourceCount;

void fldMakeResouceBasicFL1(void *typeTable, u32 batchCount) {
    fldSpawnBatch_t *batch;
    sdfVector4_t position;
    sdfVector4_t rotation;
    u32 batchIndex;
    u32 resourceCount;
    u32 resourceIndex;
    fldResHead_t *resource;
    dds3Basic_t *spawnedObject;
    fldBillConfigView_t *config;

    batch = (fldSpawnBatch_t *)typeTable;
    fldScriptResource = NULL;
    fldScriptResourceCount = 0;
    for (batchIndex = 0; batchIndex < batchCount; batchIndex++, batch++) {
        resource = batch->entries;
        resourceCount = batch->count;
        switch (batch->kind) {
        case 0:
        case 1:
            break;
        case FLD_SPAWN_BATCH_KIND_SCRIPT_RESOURCE:
            fldScriptResource = resource;
            fldScriptResourceCount = resourceCount;
            break;
        case FLD_SPAWN_BATCH_KIND_BILLBOARD_EFFECT:
            for (resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++, resource++) {
                if (resource->transform != NULL) {
                    position.x = resource->transform->pos.x;
                    position.y = resource->transform->pos.y;
                    position.z = resource->transform->pos.z;
                    position.w = 0.0f;
                    rotation.x = resource->transform->rot.x;
                    rotation.y = resource->transform->rot.y;
                    rotation.z = resource->transform->rot.z;
                    rotation.w = resource->transform->rot.w;
                } else {
                    position.x = 0.0f;
                    position.y = 0.0f;
                    position.z = 0.0f;
                    position.w = 0.0f;
                    rotation.x = 0.0f;
                    rotation.y = 0.0f;
                    rotation.z = 0.0f;
                    rotation.w = 0.0f;
                }
                config = resource->data;
                switch (config->kind) {
                case 0:
                case 1:
                case 2:
                case 3:
                    break;
                case 100:
                    spawnedObject = dds3AddESPEffectObjectBasic(resource->name, &position, &rotation, config->width * 0.5f);
                    dds3EffectObjectBasicStatOff(spawnedObject, 1);
                    break;
                }
            }
            break;
        case FLD_SPAWN_BATCH_KIND_LIGHT:
            for (resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++, resource++) {
                if (resource->transform != NULL) {
                    position.x = resource->transform->pos.x;
                    position.y = resource->transform->pos.y;
                    position.z = resource->transform->pos.z;
                    position.w = 0.0f;
                    rotation.x = resource->transform->rot.x;
                    rotation.y = resource->transform->rot.y;
                    rotation.z = resource->transform->rot.z;
                    rotation.w = resource->transform->rot.w;
                } else {
                    position.x = 0.0f;
                    position.y = 0.0f;
                    position.z = 0.0f;
                    position.w = 0.0f;
                    rotation.x = 0.0f;
                    rotation.y = 0.0f;
                    rotation.z = 0.0f;
                    rotation.w = 0.0f;
                }
                spawnedObject = dds3AddLightObjectBasic(resource->id);
                dds3SetUnitObjectId(spawnedObject, resource->name);
                dds3SetPosPoseSet(spawnedObject, &position);
                dds3GetRadiusPoseSet(spawnedObject, ((fldResourceLight_t *)resource->data)->OutsideR);
                dds3SetLightObjectResoruce(spawnedObject, resource->data);
                if ((u32)fldGlobalBase.amSBlock - 200U < 300U) {
                    ((fldLightDataView_t *)spawnedObject->workdata)->flags |= 8;
                    dds3SetUnitObjectId(spawnedObject, "BTL_DEF_LIGHT");
                }
            }
            break;
        case FLD_SPAWN_BATCH_KIND_PATH:
            for (resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++, resource++) {
                dds3AddPathBasic(resource->id, resource->data, resource->name);
            }
            break;
        case FLD_SPAWN_BATCH_KIND_COLLISION:
            for (resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++, resource++) {
                resource->id |= 0x20000;
                if (resource->transform != NULL) {
                    position.x = resource->transform->pos.x;
                    position.y = resource->transform->pos.y;
                    position.z = resource->transform->pos.z;
                    position.w = 0.0f;
                    rotation.x = resource->transform->rot.x;
                    rotation.y = resource->transform->rot.y;
                    rotation.z = resource->transform->rot.z;
                    rotation.w = resource->transform->rot.w;
                } else {
                    position.x = 0.0f;
                    position.y = 0.0f;
                    position.z = 0.0f;
                    position.w = 0.0f;
                    rotation.x = 0.0f;
                    rotation.y = 0.0f;
                    rotation.z = 0.0f;
                    rotation.w = 0.0f;
                }
            }
            break;
        }
    }
}
