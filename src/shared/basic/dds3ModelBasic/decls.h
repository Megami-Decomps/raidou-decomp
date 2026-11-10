/* Declarations of smt3-decomp src/smt3/basic/dds3ModelBasic.c, for its bodies in this directory. */
#ifndef SHARED_BASIC_DDS3MODELBASIC_H
#define SHARED_BASIC_DDS3MODELBASIC_H

#include "common.h"

#include "basic.h"

#include "model.h"

#include "motion.h"

typedef struct {
    /* 0x00 */ dds3Basic_t *pTextureBasic;
    /* 0x04 */ sdfItemArray_t *pModelDef;
    /* 0x08 */ sdfMotionPlayBook_t *pMotDef;
    /* 0x0C */ void *pTexList;
    /* 0x10 */ u8 unk10[8];
} dds3ModelBasicWorkData_t;

extern dds3Basic_t *dds3AddSceneBasicData(s32 type);

extern sdfModelRootNode_t *sdfModelMake(void *aTexList, sdfItemArray_t *aModelData);

extern sdfMotionCtrlr_t *sdfMotionCtrlrNew(sdfModelRootNode_t *aModel, sdfMotionPlayBook_t *aBook);

extern void sdfMotionChange(sdfMotionCtrlr_t *aCtrlr, s32 aCutNum, s32 aType);

dds3Basic_t *dds3AddModelBasic(s32 serialID, sdfItemArray_t *pModelDef, sdfMotionPlayBook_t *pMotDef, dds3Basic_t *pTextureBasic, char *pName);

dds3Basic_t *dds3AddModelBasic2(s32 serialID, sdfItemArray_t *pModelDef, sdfMotionPlayBook_t *pMotDef, void *pTexList, char *pName);

sdfModelRootNode_t *dds3ModelBasicMakeModel(dds3Basic_t *basic);

sdfMotionCtrlr_t *dds3ModelBasicSetMotion(dds3Basic_t *basic, sdfModelRootNode_t *model);

#endif
