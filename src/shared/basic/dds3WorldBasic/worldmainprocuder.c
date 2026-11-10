#include "shared/basic/dds3WorldBasic/decls.h"

s32 worldmainprocuder(dds3Basic_t *basic)
{
    dds3Basic_t *scene;

    scene = ((dds3WorldBasic_t *)basic->workdata)->activescene;
    if (scene != NULL) {
        dds3CallBasicMainProc(scene);
    }
    return 1;
}
