#ifndef SDFMODEL_H
#define SDFMODEL_H

#include "common.h"
#include "model.h"

u8 *sdfModelAllocTmPacket(sdfModelNode_t *aNode);
void sdfModelCopyStat(sdfModelRootNode_t *aDstRoot, sdfModelRootNode_t *aSrcRoot);
void sdfModelEvalTm(sdfModelRootNode_t *aRoot);
u32 sdfModelGetMarkPosCount(sdfModelRootNode_t *aRoot);
void sdfModelInitSystem(void);
sdfModelRootNode_t *sdfModelMake(void *aTexList, sdfModelDef_t *aModelData);
void sdfModelRootFree(sdfModelRootNode_t *aRoot, s32 aReleaseNodes, s32 aReleaseMatList);
sdfModelNode_t *sdfModelSearchNode(sdfModelRootNode_t *aRoot, s32 aId);

#endif
