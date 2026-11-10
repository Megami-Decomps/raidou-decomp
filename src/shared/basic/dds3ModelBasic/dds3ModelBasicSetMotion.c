#include "shared/basic/dds3ModelBasic/decls.h"

sdfMotionCtrlr_t *dds3ModelBasicSetMotion(dds3Basic_t *basic, sdfModelRootNode_t *model) {
    sdfMotionCtrlr_t *motion = NULL;
    dds3ModelBasicWorkData_t *work;

    if (model == NULL) {
        return NULL;
    }
    work = basic->workdata;
    if (work->pMotDef != NULL) {
        motion = sdfMotionCtrlrNew(model, work->pMotDef);
        sdfMotionChange(motion, 0, 1);
    }
    return motion;
}
