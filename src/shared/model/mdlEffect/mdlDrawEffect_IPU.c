#include "shared/model/mdlEffect/decls.h"

/* Start the IPU once its owner reaches the stored motion-frame threshold. */
void mdlDrawEffect_IPU(dds3ModelEffectCtrlr_t *effect) {
    mdlEffectResrc_IPU_t *object = effect->payload.object.object;
    if (object->playing == 0) {
        s32 minimumTime = effect->payload.object.minimumTime;
        f32 motionTime = effect->payload.object.owner->first->frame;
        if ((u32)(s32)motionTime < (u32)minimumTime) {
            return;
        }
        mdlStartEffectResrc_IPU(object, effect->payload.object.texture,
                   (mdlIpuParams_t *)effect->payload.object.attributes);
    }
}
