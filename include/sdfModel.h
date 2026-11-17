#ifndef SDFMODEL_H
#define SDFMODEL_H

#include "common.h"
#include "model.h"

/* An extra-data chunk: its four-character tag and its total size in bytes. */
typedef struct
{
    u32 tag;
    u32 size;
} sdfModelChunkHeader_t;

/* One MPOS record: 64 bytes, keyed by id, naming the node it hangs off. */
typedef struct QWORD_ALIGNED sdfModelMarkPos_t
{
    s32 nodeId;
    s32 id;
    u32 pad[2];
    sdfVector4_t pos;
    sdfVector4_t dir;
    sdfVector4_t up;
} sdfModelMarkPos_t;

u8 *sdfModelAllocTmPacket(sdfModelNode_t *aNode);
u32 sdfGetLodChunkValue(sdfModelRootNode_t *aRoot);
void sdfModelCopyStat(sdfModelRootNode_t *aDstRoot, sdfModelRootNode_t *aSrcRoot);
void sdfModelEvalTm(sdfModelRootNode_t *aRoot);
void sdfModelLinkOt_sdfModel(sdfGrOt_t **aOt, sdfModelRootNode_t *aRoot);
void *sdfModelGetExtraChunk(sdfModelRootNode_t *aRoot, u32 aChunkId);
s32 sdfModelGetNodeIndexByName(sdfModelRootNode_t *aRoot, const char *aName);
u32 sdfModelGetMarkPosCount(sdfModelRootNode_t *aRoot);
s32 sdfModelGetMarkPosMatrix_sdfModel(sdfModelRootNode_t *aRoot, s32 aId);
u32 sdfModelGetUniqueID(sdfModelRootNode_t *aRoot);
s32 sdfModelGetMarkPos_sdfModel(sdfModelRootNode_t *aRoot, s32 aId);
s32 sdfCallModelMakeMarkPos(sdfModelRootNode_t *aRoot, s32 aId);
void sdfModelInitSystem(void);
void sdfModelMakeMarkPosMatrix(sdfModelRootNode_t *aRoot, sdfModelMarkPos_t *aMark);
sdfModelMarkPos_t *sdfModelSearchMarkPosData(sdfModelRootNode_t *aRoot, s32 aId);
sdfModelRootNode_t *sdfModelMake(void *aTexList, sdfModelDef_t *aModelData);
sdfModelRootNode_t *sdfModelMakeEarly(void *aTexList, sdfModelDef_t *aModelData);
void sdfModelRootFree(sdfModelRootNode_t *aRoot, s32 aReleaseNodes, s32 aReleaseMatList);
sdfModelNode_t *sdfModelSearchNode(sdfModelRootNode_t *aRoot, s32 aId);

void sdfModelLinkShapedDrawPacket(sdfModelNode_t *aNode, sdfShapeShapedPacket_t *aDataPtr,
                                  sdfDrawTag_t *aDrawTag, s32 aId);
/* Defined in sdfSortOt. */
void sdfModelSetOtCallbacks(sdfGrOt_t *aOt);

extern f32 sdfFaceDivSoften;
extern f32 sdfFaceDivBias;
extern u8 corePktSoftenBias;
extern s32 sdfForcedAssetTextureMode;

#endif
