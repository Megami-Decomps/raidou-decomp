#include "shared/event/evtCommand/decls.h"

u32 evtCommand_MODEL_LIGHT_RGB(void)
{
    evtColorWork_t work[2];
    evtUnit_t *unit;
    u32 light;
    u32 ambient;
    u32 scale;

    unit = evtGetUnitHandle(scrGetIntPara(0));
    SDF_VU0_SETVECTOR_X(scrGetFloatPara(2));
    SDF_VU0_SETVECTOR_Y(scrGetFloatPara(3));
    SDF_VU0_SETVECTOR_Z(scrGetFloatPara(4));
    SDF_VU0_VECTOR_W_SET_ZERO();
    scale = EVT_RGBA_SCALE_128;
    MMI_RGBA_PACK_UNIT(light, scale);
    work[0].word = light;
    SDF_VU0_SETVECTOR_X(scrGetFloatPara(5));
    SDF_VU0_SETVECTOR_Y(scrGetFloatPara(6));
    SDF_VU0_SETVECTOR_Z(scrGetFloatPara(7));
    SDF_VU0_VECTOR_W_SET_ONE();
    MMI_RGBA_PACK_UNIT(ambient, scale);
    work[1].word = ambient;
    evtSetUnitLightAndAmbient(unit, scrGetIntPara(1), light, ambient);
    return 1;
}
