#include "shared/sdf/sdfDevCons/decls.h"

sdfTexHandle_t *sdfDevConsGetFontHandle(void)
{
    sdfDevConsInit();
    return sdfDebugFontHandle;
}
