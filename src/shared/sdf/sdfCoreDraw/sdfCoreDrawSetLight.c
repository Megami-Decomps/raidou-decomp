#include "shared/sdf/sdfCoreDraw/decls.h"

void sdfCoreDrawSetLight(u8 *light) {
    corePktCurrLight = EE_ADDR(light);
}
