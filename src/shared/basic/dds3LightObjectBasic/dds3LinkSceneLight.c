#include "shared/basic/dds3LightObjectBasic/decls.h"

/* vu0 routine: pack the live scene light and ambient colours */
void dds3LinkSceneLight(dds3Basic_t *basic, s32 frame)
{
    dds3LightColorWork_t work[2];
    dds3LightObjectBasicWorkData_t *data = basic->workdata;
    u32 lightColor;
    u32 ambientColor;
    u32 nextColor;

    data->Flag |= 1;
    if (frame == 0) {
        data->Flag &= ~2;
    } else {
        data->Flag |= 2;
        data->SwitchFrame = SDF_FRAMES(frame);
        SDF_VU0_LOADVECTOR(dds3LightParam.sources[0]);
        MMI_RGBA_PACK_UNIT(lightColor, 128.0f);
        work[0].word = lightColor;
        data->BaseLightRGB = lightColor;
        SDF_VU0_LOADVECTOR(&dds3LightParam.ambient);
        MMI_RGBA_PACK_UNIT(ambientColor, 128.0f);
        work[1].word = ambientColor;
        data->BaseAmbientRGB = ambientColor;
    }
}
