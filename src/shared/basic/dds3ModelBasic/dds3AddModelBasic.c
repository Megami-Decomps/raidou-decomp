#include "shared/basic/dds3ModelBasic/decls.h"

dds3Basic_t *dds3AddModelBasic(s32 serialID, sdfItemArray_t *pModelDef, sdfMotionPlayBook_t *pMotDef, dds3Basic_t *pTextureBasic, char *pName) {
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_STATICMODEL);
    dds3ModelBasicWorkData_t *work = basic->workdata;

    work->pMotDef = pMotDef;
    basic->serialID = serialID;
    basic->name = pName;
    work->pTextureBasic = pTextureBasic;
    work->pModelDef = pModelDef;
    work->pTexList = NULL;
    return basic;
}
