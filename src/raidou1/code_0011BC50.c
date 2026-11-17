#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "dds3Basic.h"
#include "dds3SceneBasic.h"
#include "model.h"
#include "dds3ObjectBase.h"

void dds3RefreshStoredVec3(dds3Basic_t *object)
{
    dds3PoseSet_t *poseset = object->poseset;
    dds3MatterObjectBasicWorkData_t *work = object->workdata;

    work->base_pos[0] = poseset->pos.x;
    work->base_pos[1] = poseset->pos.y;
    work->base_pos[2] = poseset->pos.z;
}
