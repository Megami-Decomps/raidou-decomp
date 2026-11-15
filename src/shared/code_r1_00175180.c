#include "common.h"
#include "fld.h"
#include "fldNpcPath.h"
#include "sdk_libc.h"

/* NON_MATCHING: 22 of 113 words match. Retail adds 4 to the DDS3_GBWK4 base separately and allocates registers differently. */
#ifdef NON_MATCHING
mapNpcPathControl_t *mapNpcPathMakeControl(s16 uniq) {
    mapNpcPathSlot_t *slot = &DDS3_GBWK4[uniq];
    mapNpcPathTableInfo_t *info = slot->info;
    mapNpcPathControl_t *control;
    mapNpcPathData_t *pTrans = NULL;
    mapNpcPathData_t *pData;
    f32 key;
    u32 i;

    if (info->nPathTable == 0) {
        return NULL;
    }
    control = &slot->control;
    memset(control, 0, sizeof(*control));
    control->mode = 0;
    control->frame = 0;
    control->frameMax = 0;
    for (i = 0; i < info->nPathTable; i++) {
        pData = info->pPathTable[i].pData;
        if (info->pPathTable[i].type == 0) {
            control->flag |= 1;
            control->pTransrate = pData;
            pTrans = pData;
            mapNpcPathMake_KeyFrame(pData->nKey, info->spd, pData);
        } else if (info->pPathTable[i].type == 2) {
            control->flag |= 2;
            control->pRotation = pData;
            mapNpcPathMake_Rot(pData->nKey, pTrans, pData);
        }
        key = pData->pKey[pData->nKey - 1];
        if (control->frameMax < key) {
            control->frameMax = key;
        }
    }
    return control;
}
#else
INCLUDE_ASM(const s32, "code_r1_00175180", mapNpcPathMakeControl);
#endif
