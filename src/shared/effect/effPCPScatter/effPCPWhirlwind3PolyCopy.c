#include "shared/effect/effPCPScatter/decls.h"

effPCPWhirlwind3Poly_tag *effPCPWhirlwind3PolyCopy(effPCPWhirlwind3Poly_tag *work)
{
    effPCPWhirlwind3Poly_tag *child;

    child = effPCPWhirlwind3PolyCreate(&work->param, NULL);
    dds3WhirlwindPolyListCopyTex(child->hPoly, work->hPoly);
    return child;
}
