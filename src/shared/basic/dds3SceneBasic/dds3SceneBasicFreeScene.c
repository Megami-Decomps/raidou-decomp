#include "shared/basic/dds3SceneBasic/decls.h"

void dds3SceneBasicFreeScene(dds3Basic_t *pBasic)
{
    dds3SceneBasic_t *work;

    work = pBasic->workdata;
    fldFreeFile();
    work->hFileHandle = 0;
}
