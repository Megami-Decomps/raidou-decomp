#include "shared/kernel/dds3KernelDraw/decls.h"

void eglInit(void)
{
    if (eglMaterial == NULL) {
        eglMaterial = sdfMatNew();
        eglMaterial->constant = 1.0f;
    }
}
