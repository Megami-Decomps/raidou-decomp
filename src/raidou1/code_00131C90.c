#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "event.h"
#include "scr.h"
#include "dds3SceneBasic.h"
#include "dds3KernelCore.h"
#include "field.h"
#include "fldFileResolver_00132080.h"
#include "dds3Basic.h"
#include "dds3EventBasic.h"
#include "dds3PathBasic.h"
#include "dds3PointBasic.h"
#include "dds3CameraObjectBasic.h"

/* NON_MATCHING: 121 of 252 words match.  */
#ifdef NON_MATCHING
/* Resolved FL2 event payload: the event data follows the kind word. */
typedef struct fldSpawnEventDataFL2_t {
    u32 kind;
    void *data;
} fldSpawnEventDataFL2_t;

void fldMakeResouceBasicFL2(fldSpawnBatch_t *batch, u32 batchCount, s32 mode) {
    sdfVector4_t position;
    sdfVector4_t rotation;
    u32 batchIndex;
    u32 resourceCount;
    u32 resourceIndex;
    fldResHead_t *resource;
    dds3Basic_t *spawnedObject;

    if (mode == 0) {
        g_pHitData = NULL;
        g_nHitData = 0;
    }
    for (batchIndex = 0; batchIndex < batchCount; batchIndex++, batch++) {
        resource = batch->entries;
        resourceCount = batch->count;
        switch (batch->kind) {
        case 0:
        case 1:
        case 2:
        case 5:
        case 8:
        case 11:
            break;
        case FLD_SPAWN_BATCH_KIND_COLLISION:
            if (mode != 0) {
                break;
            }
            g_pHitData = resource;
            g_nHitData = resourceCount;
            for (resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++, resource++) {
                resource->id |= 0x10000;
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
        case FLD_SPAWN_BATCH_KIND_EVENT:
            for (resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++, resource++) {
                if (mode != 0) {
                    resource->id |= 0x800000;
                } else {
                    MakeResouceBasicFL2Data++;
                }
                dds3AddEventBasic(resource->id, ((fldSpawnEventDataFL2_t *)resource->data)->data,
                                  resource->name);
            }
            break;
        case FLD_SPAWN_BATCH_KIND_PATH:
            for (resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++, resource++) {
                if (mode != 0) {
                    resource->id |= 0x800000;
                }
                dds3AddPathBasic(resource->id, resource->data, resource->name);
            }
            break;
        case FLD_SPAWN_BATCH_KIND_POINT:
            for (resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++, resource++) {
                if (mode != 0) {
                    resource->id |= 0x800000;
                }
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
                switch (((fldResPut_t *)resource->data)->type) {
                case 0:
                    dds3AddPointBasic(resource->id, (fldResourcePoint_t *)resource->transform,
                                      resource->name);
                    break;
                }
            }
            break;
        case FLD_SPAWN_BATCH_KIND_CAMERA:
            for (resourceIndex = 0; resourceIndex < resourceCount; resourceIndex++, resource++) {
                if (mode != 0) {
                    resource->id |= 0x800000;
                }
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
                spawnedObject = dds3AddCameraObjectBasicTypeFree(resource->id, &position, &rotation);
                ((dds3CameraObjectBasicWorkData_t *)spawnedObject->workdata)->fovy = *(f32 *)resource->data;
                dds3SetUnitObjectId(spawnedObject, resource->name);
            }
            break;
        case FLD_SPAWN_BATCH_KIND_UNSUPPORTED:
            for (;;) {
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_00131C90", fldMakeResouceBasicFL2);
#endif
