#include "shared/effect/effPCPScatter/decls.h"

effPCPWhirlwind2Poly_tag *effPCPWhirlwind2PolyCopy(effPCPWhirlwind2Poly_tag *work)
{
    effPCPWhirlwind2Poly_tag *child;

    child = effPCPWhirlwind2PolyCreate(&work->param, NULL);
    dds3WhirlwindPolyListCopyTex(child->hPoly, work->hPoly);
    return child;
}
