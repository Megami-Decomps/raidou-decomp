#ifndef DDS3MODELBASIC_H
#define DDS3MODELBASIC_H

#include "common.h"
#include "basic.h"
#include "model.h"
#include "motion.h"

sdfModelRootNode_t *dds3ModelBasicMakeModel(dds3Basic_t *basic);
sdfMotionCtrlr_t *dds3ModelBasicSetMotion(dds3Basic_t *basic, sdfModelRootNode_t *model);

#endif
