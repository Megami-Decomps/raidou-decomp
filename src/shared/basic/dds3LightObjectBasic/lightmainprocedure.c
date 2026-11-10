#include "shared/basic/dds3LightObjectBasic/decls.h"

/* vu0 routine: blend the stored light and ambient colours toward the target */
s32 lightmainprocedure(dds3Basic_t *basic)
{
    sdf3DLight_t *lights[3];
    dds3LightColorWork_t work[4];
    dds3LightObjectBasicWorkData_t *data;
    u32 flag;
    f32 ratio;
    u32 lightColor;
    u32 ambientColor;
    u32 nextColor;

    memset(lights, 0, sizeof(lights));
    data = basic->workdata;
    flag = data->Flag;
    if ((flag & 1) != 0) {
        if ((flag & 2) != 0) {
            if (data->SwitchFrame == 0) {
                data->Flag = flag & ~2;
                ratio = 1.0f;
            } else {
                u16 frames = data->SwitchFrame;

                ratio = 1.0f / (f32)frames;
                data->SwitchFrame = frames - 1;
            }
            work[0].word = data->BaseLightRGB;
            MMI_RGBA_UNPACK(&work[0], 0.0078125f);
            SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
            SDF_VU0_LOADVECTOR_B(&data->Light.color);
            SDF_VU0_SCALEVECTOR_B_BITS(ratio);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_COPYVECTOR_A_TO_C();
            MMI_RGBA_PACK_UNIT(lightColor, 128.0f);
            nextColor = data->BaseAmbientRGB;
            work[1].word = lightColor;
            data->BaseLightRGB = lightColor;
            work[2].word = nextColor;
            MMI_RGBA_UNPACK(&work[2], 0.0078125f);
            SDF_VU0_SCALEVECTOR_BITS(1.0f - ratio);
            SDF_VU0_LOADVECTOR_B(&data->Ambient);
            SDF_VU0_SCALEVECTOR_B_BITS(ratio);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_COPYVECTOR_A_TO_B();
            MMI_RGBA_PACK_UNIT(ambientColor, 128.0f);
            work[3].word = ambientColor;
            data->BaseAmbientRGB = ambientColor;
            SDF_VU0_COPYVECTOR_C_TO_A();
        } else {
            SDF_VU0_LOADVECTOR(&data->Light.color);
            SDF_VU0_LOADVECTOR_B(&data->Ambient);
        }
        SDF_VU0_STOREVECTOR(dds3LightParam.sources[0]);
        SDF_VU0_STOREVECTOR_B(&dds3LightObjectAmbient);
    }
    lights[0] = &data->Light;
    sdf3DMakeLightParallelPacket(data->LightPacket, lights, &data->Ambient);
    return 1;
}
