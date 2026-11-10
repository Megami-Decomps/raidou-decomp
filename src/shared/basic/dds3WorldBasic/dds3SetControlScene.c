#include "shared/basic/dds3WorldBasic/decls.h"

void dds3SetControlScene(dds3Basic_t *scene)
{
    if (dds3TheWorld != NULL) {
        ((dds3WorldBasic_t *)dds3TheWorld->workdata)->controlscene = scene;
    }
}
