#include "shared/basic/dds3LightObjectBasic/decls.h"

void dds3SetLightObjectPathResoruce(dds3Basic_t *basic, fldResourceLightPath_t *resource)
{
    dds3LightObjectBasicWorkData_t *work = basic->workdata;

    work->InsideR = resource->InsideR;
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
