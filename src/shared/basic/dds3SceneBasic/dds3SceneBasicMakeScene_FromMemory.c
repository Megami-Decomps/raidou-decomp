#include "shared/basic/dds3SceneBasic/decls.h"

void dds3SceneBasicMakeScene_FromMemory(dds3Basic_t *pBasic, u32 field, u32 area,
                                       void *pF1, void *pF2,
                                       void *pTB, u32 pBF)
{
    dds3SceneBasic_t *work;
    u32 handle;

    work = pBasic->workdata;
    handle = fldLoadFile_FromMemory(field, area, pF1, pF2, pTB, pBF);
    work->hFileHandle = handle;
}
