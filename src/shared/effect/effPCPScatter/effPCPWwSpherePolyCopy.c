#include "shared/effect/effPCPScatter/decls.h"

effPCPWwSpherePoly_tag *effPCPWwSpherePolyCopy(effPCPWwSpherePoly_tag *work)
{
    effPCPWwSpherePoly_tag *child;

    child = effPCPWwSpherePolyCreate(&work->param, NULL);
    dds3WhirlwindPolyListCopyTex(child->hPoly, work->hPoly);
    return child;
}
