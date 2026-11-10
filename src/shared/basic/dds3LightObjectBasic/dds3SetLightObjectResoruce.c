#include "shared/basic/dds3LightObjectBasic/decls.h"

void dds3SetLightObjectResoruce(dds3Basic_t *basic, fldResourceLight_t *resource)
{
    dds3LightObjectBasicWorkData_t *work = basic->workdata;

    work->Flag = 0;
    if (resource->Flag & 1) {
        work->Flag = 1;
    }
    if (resource->Flag & 2) {
        work->Flag |= 4;
    }

    work->InsideR = resource->InsideR;
    work->AnimeType = resource->AnimeType;
    work->OutsideR = resource->OutsideR;
    work->Soften = resource->Soften;
    work->Bias = resource->Bias;
    work->Light.color.x = resource->DiffuseR;
    work->Light.color.y = resource->DiffuseG;
    work->Light.color.z = resource->DiffuseB;
    work->Light.color.w = 0.0f;
    work->Ambient.x = resource->AmbientR;
    work->Ambient.y = resource->AmbientG;
    work->Ambient.z = resource->AmbientB;
    work->Ambient.w = 1.0f;
}
