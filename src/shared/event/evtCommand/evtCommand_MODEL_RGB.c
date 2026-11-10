#include "shared/event/evtCommand/decls.h"

u32 evtCommand_MODEL_RGB(void)
{
    evtColorWork_t work[1];
    evtUnit_t *unit;
    u32 color;
    s32 duration;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    SDF_VU0_SETVECTOR_X(scrGetFloatPara(2));
    SDF_VU0_SETVECTOR_Y(scrGetFloatPara(3));
    SDF_VU0_SETVECTOR_Z(scrGetFloatPara(4));
    SDF_VU0_VECTOR_W_SET_ZERO();
    MMI_RGBA_PACK_128_S2(color);
    work[0].word = color;
    duration = scrGetIntPara(1);
    /* Qualified cc1 loop notes 106/126 keep the unit argument before the
     * duration argument; the no-block control reverses those two copies.
     * See docs/levers/control-flow.md. */
    do {
        evtSetUnitColorRGB(unit, duration, color);
    } while (0);
    return 1;
}
