/* Declarations of smt3-decomp src/smt3/sdf/sdfModel.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFMODEL_H
#define SHARED_SDF_SDFMODEL_H

#include "common.h"

#include "libsdf.h"

#include "model.h"

#include "draw.h"

#include "ps2asm.h"

#include "eeregs.h"

/* The serialized node-def block has a 16-byte count/padding prefix. */
typedef struct
{
    s32 count;
    u32 pad[3];
} sdfModelNodeDefList_t;

extern void FromEuler(f32 aRotateX, f32 aRotateY, f32 aRotateZ);

extern void ToMatrix(void);

extern void MultMatrixRevNoArg(void);

extern void MultMatrixRev(void *aMatrix);

extern void sdf3DPrimSetTmPacket(void *aPacket);

extern void sdfModelSetOtCallbacks(sdfGrOt_t *aOt);

extern void *sdfChipAllocDirect(s32 aSize);

extern void *sdfChipAllocZeroDirect(s32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern sdfItemArray_t *sdfItemArrayNew(s32 aInitCapa, s32 aElemSize, s32 aGrowStep);

extern void sdfItemArrayFree(void *aArray);

extern void sdfItemArrayGrow(void *aArray);

extern void func_002BFF98(void *aArray, s32 aCount);

extern void sdfDelayedResrcInitCB(sdfDelayedResrcCB_t *aCB, sdfDelayedResrcFreeFunc aFreeFunc);

extern void sdfDelayedResrcEntry(sdfDelayedResrcCB_t *aCB, void *aItem);

extern sdfDelayedResrcCB_t sdfModelDelayedResrcCB;

extern sdfMatList_t *sdfMatListMake(void *aTexList, u32 *aData);

extern void sdfMatListFreeResrc(void *aList, s32 aDelayed);

extern void sdfMatListCopyStat(void *aDst, void *aSrc);

extern sdfDmaTag_t *sdfMatMakePacket(sdfMat_t *aMat, sdfDmaTag_t *aBuffer, s32 aId);

extern volatile u8 sdfFrameId;

extern f32 sdfFaceDivSoften;

extern f32 sdfFaceDivBias;

extern const char sdfModelDefaultName[];

extern void *sdfGrAllocPacketBuff(s32 aSize);

extern void sdfDrawTagLink2(sdfDrawTag_t *aTag, addr_t aHeadPacket, addr_t aTailPacket);

extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

extern void sdfCoreDrawScanOneSkinNodes(sdfModelRootNode_t *root, void *p);

extern void *func_002B2628(void *aData, u32 aTag);

extern void sdfModelEvalTm2(sdfModelRootNode_t *aRoot, s32 aFrameId);

extern void sdfModelLinkDrawPacket(sdfModelNode_t *aNode, void *aShape, s32 aOtIndex, s32 aIndexFrom,
                                   s32 aId);

extern void sdfModelNodeBaseSet(sdfModelNode_t *aNode, sdfModelNodeDef_t *aDef);

extern sdfModelNode_t *sdfModelNodeNew(void);

extern void sdfModelMakeLocalTm(sdfModelNode_t *aNode);

extern sdfModelRootNode_t *sdfModelRootNew(void);

extern void sdfModelAllocNodeList(sdfModelRootNode_t *aRoot, s32 aLeng);

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);

extern void sdfPrimSetOpaquePacket(sdfPrimTransPacket_t *aPkt);

extern void sdfPrimSetSemiTransPacket(sdfPrimTransPacket_t *aPkt);

extern void sdfPrimSetAddTransPacket(sdfPrimTransPacket_t *aPkt);

extern void sdfPrimSetSubTransPacket(sdfPrimTransPacket_t *aPkt);

/* The shaped packet's header quadwords: the cnt tag, the two blend weights
 * VU1 reads and the UNPACK that follows them. */
typedef struct QWORD_ALIGNED
{
    sdfDmaTag_t dmatag;
    f32 weight[2];
    u32 code0;
    u32 code1;
} sdfModelShapedHeader_t;

/* One frame's 128-byte VU1 transform packet. The 4-aligned element type is
 * what gives retail's runtime 8-byte-alignment dispatch over the copy. */
/* sdfModelCopyStat below keeps its stride as (frame << 7) rather than a
 * sizeof: the multiply there changes two words of the copy's addressing. */
typedef struct
{
    s32 words[0x80 / 4];
} sdfModelTmImage_t;

/* An extra-data chunk: its four-character tag and its total size in bytes. */
typedef struct
{
    u32 tag;
    u32 size;
} sdfModelChunkHeader_t;

/* One MPOS record: 64 bytes, keyed by id, naming the node it hangs off. */
typedef struct QWORD_ALIGNED
{
    s32 nodeId;
    s32 id;
    u32 pad[2];
    sdfVector4_t pos;
    sdfVector4_t dir;
    sdfVector4_t up;
} sdfModelMarkPos_t;

void sdfModelRootFreeResrc(sdfModelRootNode_t *aRoot);

void sdfModelPacketFree(sdfModelNode_t *aNode);

void sdfModelNodeFree(sdfModelNode_t *aNode);

u8 *sdfModelAllocTmPacket(sdfModelNode_t *aNode);

void sdfModelNodeUnlink(sdfModelNode_t *aNode);

void sdfModelNodeSetParent(sdfModelNode_t *aNode, sdfModelNode_t *aParent);

void sdfModelNodeAdd(sdfModelRootNode_t *aRoot, sdfModelNode_t *aParent, sdfModelNode_t *aNode);

sdfModelNode_t *sdfModelSearchNode(sdfModelRootNode_t *aRoot, s32 aId);

sdfDmaTag_t *sdfModelSetMatRefPacket(sdfModelRootNode_t *aRoot, s32 aMatIndex, sdfDmaTag_t *aBuffer,
                                     s32 aId);

void sdfModelMakeMorphList(sdfModelRootNode_t *root, s32 morphCount);

sdfModelRootNode_t *sdfModelMakeRoot(void *aTexList, sdfModelDef_t *aModelData);

void sdfModelNodeSet(sdfModelNode_t *aNode, sdfModelNodeDef_t *aDef);

void sdfModelMorphListCopyStat(sdfModelMorphList_t *aDst, sdfModelMorphList_t *aSrc);

void *sdfModelGetExtraChunk(sdfModelRootNode_t *aRoot, u32 aChunkId);

sdfModelMarkPos_t *sdfModelSearchMarkPosData(sdfModelRootNode_t *aRoot, s32 aId);

void sdfModelMakeMarkPosMatrix(sdfModelRootNode_t *aRoot, sdfModelMarkPos_t *aMark);

void sdfModelMakeMarkPos(sdfModelRootNode_t *aRoot, sdfModelMarkPos_t *aMark);

void sdfModelInitSystem(void);

/* vu0 routine: native SDK vector/matrix operations around model state. */
sdfModelNode_t *sdfModelNodeNew(void);

u8 *sdfModelAllocTmPacket(sdfModelNode_t *aNode);

void sdfModelPacketFree(sdfModelNode_t *aNode);

void sdfModelNodeFree(sdfModelNode_t *aNode);

/* vu0 routine: native SDK vector/matrix operations around model state. */
sdfModelRootNode_t *sdfModelRootNew(void);

void sdfModelRootFreeResrc(sdfModelRootNode_t *aRoot);

void sdfModelRootFree(sdfModelRootNode_t *aRoot, s32 aReleaseNodes, s32 aReleaseMatList);

void sdfModelAllocNodeList(sdfModelRootNode_t *aRoot, s32 aLeng);

void sdfModelMakeNodeList(sdfModelRootNode_t *aRoot);

void sdfModelNodeAdd(sdfModelRootNode_t *aRoot, sdfModelNode_t *aParent, sdfModelNode_t *aNode);

void sdfModelNodeUnlink(sdfModelNode_t *aNode);

void sdfModelNodeSetParent(sdfModelNode_t *aNode, sdfModelNode_t *aParent);

sdfModelNode_t *sdfModelSearchNode(sdfModelRootNode_t *aRoot, s32 aId);

sdfDmaTag_t *sdfModelSetTmRefPacket(sdfModelNode_t *aNode, sdfDmaTag_t *aBuffer, s32 aId);

sdfDmaTag_t *sdfModelSetTermPacket(sdfDmaTag_t *aBuffer);

sdfDmaTag_t *sdfModelSetMatRefPacket(sdfModelRootNode_t *aRoot, s32 aMatIndex, sdfDmaTag_t *aBuffer,
                                     s32 aId);

s32 sdfModelGetDrawPacketSize(const u32 *aList, s32 *aShapeCount, s32 *aExtSize);

sdfDmaTag_t *sdfModelSetNormalDrawPacket(sdfModelNode_t *aNode, sdfShapePacket_t *aDataPtr,
                                         sdfDmaTag_t *aBuffer, s32 aId);

sdfModelPacketHeader_tag *sdfModelNewPacketHeader(sdfModelNode_t *aNode, s32 aOtIndex, s32 aId);

/* vu0 routine: native SDK vector/matrix operations around model state. */
void sdfModelMakeLocalTm(sdfModelNode_t *aNode);

/* vu0 routine: native SDK vector/matrix operations around model state. */
void sdfModelNodeBaseSet(sdfModelNode_t *aNode, sdfModelNodeDef_t *aDef);

void sdfModelNodeSet(sdfModelNode_t *aNode, sdfModelNodeDef_t *aDef);

void sdfModelNodeSetShape(sdfModelNode_t *aNode, void *aData, s32 aType);

void sdfModelMakeMorphList(sdfModelRootNode_t *root, s32 morphCount);

sdfModelRootNode_t *sdfModelMakeRoot(void *aTexList, sdfModelDef_t *aModelData);

sdfModelRootNode_t *sdfModelMake(void *aTexList, sdfModelDef_t *aModelData);

sdfModelRootNode_t *sdfModelMakeEarly(void *aTexList, sdfModelDef_t *aModelData);

/* vu0 routine: native SDK vector/matrix operations around model state. */
void sdfModelEvalNode(sdfModelNode_t *aNode, const f32 (*aParentTm)[4], s32 aFrameId);

/* vu0 routine: native SDK vector/matrix operations around model state. */
void sdfModelEvalTm2(sdfModelRootNode_t *aRoot, s32 aFrameId);

void sdfModelEvalTm(sdfModelRootNode_t *aRoot);

void sdfModelOtInit(sdfModelOt_t *aModelOt, sdfGrOt_t *aOt0, sdfGrOt_t *aOt1,
                    sdfGrOt_t *aOt2, sdfGrOt_t *aOt3);

/* vu0 routine: native SDK vector/matrix operations around model state. */
s32 sdfModelCheckBounds(const f32 *aMinMax);

extern void sdfMatListUpdatePacket(sdfMatList_t *aMatList, s32 aId);

extern void *sdf3DLightDefault;

extern void *sdf3DFogDefault;

extern u8 corePktSoftenBias;

extern u8 sdf3DScreenTm[];

extern sdfDrawTag_t *sdfPrimAllocDrawTag(s32 (*aAllocator)(s32));

extern void sdfDrawTagLinkRef(sdfDrawTag_t *aTag, addr_t aPacket);

extern void sdfDrawTagLinkCall(sdfDrawTag_t *aTag, addr_t aPacket);

extern void sdfCoreDrawSetZBias(f32 z);

extern void sdfCoreDrawSetLight(void *light);

extern void sdfCoreDrawSetFog(void *fog);

extern void *sdfCorePacketGen(sdfModelNode_t *node, void *packet_p, u32 attr_mask, u32 color,
                              f32 fdSoften, f32 fdBias, s32 frame_id, s32 reverse);

/* The 0xC0-byte packet a temp-linked node builds: the tm REF, the 128 bytes
 * VU1 reads through it, and the draw parameters that close it. */
typedef struct QWORD_ALIGNED
{
    sdfDmaTag_t tmTag;
    u8 tm[0x80];
    sdfModelDrawParmPacket_t parm;
} sdfModelTempDrawPacket_t;

/* DDS engine donor: dds-decomp (redthing1), sdfModelBuildNodeResourcePackets. */
/* vu0 routine: each node's screen transform, and its w lane as the sort key. */
void sdfModelLinkOt(sdfGrOt_t **aOt, sdfModelRootNode_t *aRoot);

void sdfModelMorphListCopyStat(sdfModelMorphList_t *aDst, sdfModelMorphList_t *aSrc);

/* vu0 routine: native SDK vector/matrix operations around model state. */
void sdfModelCopyStat(sdfModelRootNode_t *aDstRoot, sdfModelRootNode_t *aSrcRoot);

void sdfModelSetDrawEnv(sdfModelDrawEnv_t *aEnv);

void sdfModelDrawEnvLinkOt(sdfGrOt_t **aOt, sdfModelDrawEnv_t *aEnv);

void *func_002B2628(void *data, u32 chunkId);

void *sdfModelGetExtraChunk(sdfModelRootNode_t *aRoot, u32 aChunkId);

s32 sdfModelGetNodeIndexByName(sdfModelRootNode_t *aRoot, const char *aName);

const char *sdfModelGetNodeName(sdfModelRootNode_t *aRoot, s32 aIndex);

u32 sdfModelGetMarkPosCount(sdfModelRootNode_t *aRoot);

/* vu0 routine: native SDK vector/matrix operations around model state. */
void sdfModelMakeMarkPosMatrix(sdfModelRootNode_t *aRoot, sdfModelMarkPos_t *aMark);

/* vu0 routine: native SDK vector/matrix operations around model state. */
void sdfModelMakeMarkPos(sdfModelRootNode_t *aRoot, sdfModelMarkPos_t *aMark);

sdfModelMarkPos_t *sdfModelSearchMarkPosData(sdfModelRootNode_t *aRoot, s32 aId);

s32 sdfModelGetMarkPosMatrix(sdfModelRootNode_t *aRoot, s32 aId);

s32 sdfModelGetMarkPos(sdfModelRootNode_t *aRoot, s32 aId);

void sdfModelSetFaceDivParm(sdfModelRootNode_t *aRoot, f32 aSoften, f32 aBias);

void sdfModelResetFaceDivParm(sdfModelRootNode_t *aRoot);

f32 sdfModelGetFaceDivSoften(sdfModelRootNode_t *aRoot);

f32 sdfModelGetFaceDivBias(sdfModelRootNode_t *aRoot);

#endif
