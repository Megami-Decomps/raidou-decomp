#include "shared/basic/dds3ModelBasic/decls.h"

dds3Basic_t *dds3AddModelBasic2(s32 serialID, sdfItemArray_t *pModelDef, sdfMotionPlayBook_t *pMotDef, void *pTexList, char *pName) {
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_STATICMODEL);
    dds3ModelBasicWorkData_t *work = basic->workdata;

    basic->serialID = serialID;
    work->pTexList = pTexList;
    work->pModelDef = pModelDef;
    work->pMotDef = pMotDef;
    basic->name = pName;
    return basic;
}
