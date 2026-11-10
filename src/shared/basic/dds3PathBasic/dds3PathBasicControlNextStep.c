#include "shared/basic/dds3PathBasic/decls.h"

/* vu0 routine: interpolate the light channel's radii and colors. */
void dds3PathBasicControlNextStep(dds3PathBasicControl_t *work, fldResourceLightPath_t *output)
{
    u32 segment;
    f32 weight;
    fldResourcePathData_t *keys;
    fldResourceLightPath_t *entries;

    if (work->flag & DDS3_PATH_WORLD_TRANSFORM_CHANNEL) {
        keys = work->pLight;
        dds3PathEvalSpline(&segment, &weight, keys, work->frame);
        entries = (fldResourceLightPath_t *)keys->data.transform;
        SDF_VU0_SETVECTOR_X(entries[segment + 1].InsideR);
        SDF_VU0_SETVECTOR_Y(entries[segment + 1].OutsideR);
        SDF_VU0_SETVECTOR_Z(entries[segment + 1].Soften);
        SDF_VU0_SETVECTOR_W(entries[segment + 1].Bias);
        SDF_VU0_SCALEVECTOR_V0(weight);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_SETVECTOR_X(entries[segment].InsideR);
        SDF_VU0_SETVECTOR_Y(entries[segment].OutsideR);
        SDF_VU0_SETVECTOR_Z(entries[segment].Soften);
        SDF_VU0_SETVECTOR_W(entries[segment].Bias);
        SDF_VU0_SCALEVECTOR_V0(1.0f - weight);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_GETVECTOR_X(output->InsideR);
        SDF_VU0_GETVECTOR_Y(output->OutsideR);
        SDF_VU0_GETVECTOR_Z(output->Soften);
        SDF_VU0_GETVECTOR_W(output->Bias);
        SDF_VU0_SETVECTOR_X(entries[segment + 1].DiffuseR);
        SDF_VU0_SETVECTOR_Y(entries[segment + 1].DiffuseG);
        SDF_VU0_SETVECTOR_Z(entries[segment + 1].DiffuseB);
        SDF_VU0_SCALEVECTOR_V0(weight);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_SETVECTOR_X(entries[segment].DiffuseR);
        SDF_VU0_SETVECTOR_Y(entries[segment].DiffuseG);
        SDF_VU0_SETVECTOR_Z(entries[segment].DiffuseB);
        SDF_VU0_SCALEVECTOR_V0(1.0f - weight);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_GETVECTOR_X(output->DiffuseR);
        SDF_VU0_GETVECTOR_Y(output->DiffuseG);
        SDF_VU0_GETVECTOR_Z(output->DiffuseB);
        SDF_VU0_SETVECTOR_X(entries[segment + 1].AmbientR);
        SDF_VU0_SETVECTOR_Y(entries[segment + 1].AmbientG);
        SDF_VU0_SETVECTOR_Z(entries[segment + 1].AmbientB);
        SDF_VU0_SCALEVECTOR_V0(weight);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_SETVECTOR_X(entries[segment].AmbientR);
        SDF_VU0_SETVECTOR_Y(entries[segment].AmbientG);
        SDF_VU0_SETVECTOR_Z(entries[segment].AmbientB);
        SDF_VU0_SCALEVECTOR_V0(1.0f - weight);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_GETVECTOR_X(output->AmbientR);
        SDF_VU0_GETVECTOR_Y(output->AmbientG);
        SDF_VU0_GETVECTOR_Z(output->AmbientB);
    } else {
        memset(output, 0, sizeof(*output));
    }
}
