#include "shared/event/evtEventPack/decls.h"

/* Release the event task's owned handles, then free its state.
 * File I/O is waited on even when the user-value handle is zero. */
void evtWaitEventPackLoad(dds3ProcessID_t *task) {
    void *stateHandle = dds3GetProcessData(task);
    evtPackProcessData_t *packState = (evtPackProcessData_t *)stateHandle;

    fileSyncAll();
    if (stateHandle != 0) {
        if (packState->effect72 != 0) {
            Eve_DistortionBlurSetDefTex();
            sdfTexFreeDelayed(packState->effect72);
        }
        if (packState->effect71 != 0) {
            Eve_PartialMultiBlurSetDefTex();
            sdfTexFreeDelayed(EE_ADDR_AS(struct sdfTexHandle_t *, packState->effect71));
        }
        if (packState->effect76 != 0) {
            Eve_RippleBlurSetDefTex();
            sdfTexFreeDelayed(EE_ADDR_AS(struct sdfTexHandle_t *, packState->effect76));
        }
        if (packState->effect75 != 0) {
            Eve_PartialMultiFilterSetDefTex();
            sdfTexFreeDelayed(EE_ADDR_AS(struct sdfTexHandle_t *, packState->effect75));
        }
        if (packState->pendingRequest != NULL) {
            fileHandleFree(packState->pendingRequest);
        }
        if (packState->resourceAllocation != NULL) {
            sdfMemFreeDelayedDirect(packState->resourceAllocation);
        }
        if (packState->sceneAllocation1 != 0) {
            sdfMemFreeDirect(EE_ADDR_AS(struct sdfMemHandle_t *, (u32)(packState->sceneAllocation1)));
        }
        if (packState->sceneAllocation2 != 0) {
            sdfMemFreeDirect(EE_ADDR_AS(struct sdfMemHandle_t *, (u32)(packState->sceneAllocation2)));
        }
    }
    sdfChipFreeDirect((void *)stateHandle);
}
