#include "shared/event/evtCommand/decls.h"

u32 evtCommand_MODEL_A(void)
{
    evtColorWork_t work[1];
    evtUnit_t *unit;
    u32 color;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    SDF_VU0_LOADIDENTITYVECTOR();
    SDF_VU0_SETVECTOR_W(scrGetFloatPara(2));
    MMI_RGBA_PACK_128_S2(color);
    work[0].word = color;
    evtSetUnitColorA(unit, scrGetIntPara(1), color);
    return 1;
}
