#include "shared/basic/dds3ModelBasic/decls.h"

sdfModelRootNode_t *dds3ModelBasicMakeModel(dds3Basic_t *basic) {
    dds3ModelBasicWorkData_t *work = basic->workdata;
    sdfModelRootNode_t *model;

    if (work->pTexList != NULL) {
        model = sdfModelMake(work->pTexList, work->pModelDef);
    } else {
        model = sdfModelMake(work->pTextureBasic->workdata, work->pModelDef);
    }
    return model;
}
