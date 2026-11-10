#include "shared/basic/dds3MatterObjectBasic/decls.h"

s32 dds3MatterObjectBasicConstructor(dds3Basic_t *aPtr) {
    dds3MatterObjectBasicWorkData_t *work;
    void *allocation;

    dds3InitPoseSet(aPtr);
    allocation = sdfChipAllocDirect(sizeof(dds3MatterObjectBasicWorkData_t));
    aPtr->workdata = allocation;
    memset(allocation, 0, sizeof(dds3MatterObjectBasicWorkData_t));
    work = aPtr->workdata;
    work->pObjectBaseControlWorkData = dds3ObjectBaseInitControlWorkData(aPtr);
    work->hojiID = 0;
    work->alpmode = 0;
    work->OtLinkIdx = -1;
    work->base_pos[0] = 0.0f;
    work->base_pos[1] = 0.0f;
    work->base_pos[2] = 0.0f;
    work->base_pos[3] = 1.0f;
    work->move_pos[0] = 0.0f;
    work->move_pos[1] = 0.0f;
    work->move_pos[2] = 0.0f;
    work->move_pos[3] = 1.0f;
    dds3ObjectBaseOnFlag(aPtr, 0xE2);
    return 1;
}
