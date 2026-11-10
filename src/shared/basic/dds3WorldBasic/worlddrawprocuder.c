#include "shared/basic/dds3WorldBasic/decls.h"

s32 worlddrawprocuder(dds3Basic_t *basic)
{
    dds3Basic_t *scene;

    scene = ((dds3WorldBasic_t *)basic->workdata)->activescene;
    if (scene != NULL) {
        dds3CallBasicDrawProc(scene);
    }
    return 1;
}
