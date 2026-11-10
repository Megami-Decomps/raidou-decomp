#include "shared/effect/effPCPScatter/decls.h"

effPCPWhirlwindPoly_tag *effPCPWhirlwindPolyCopy(effPCPWhirlwindPoly_tag *work) {
    effPCPWhirlwindPoly_tag *child;

    child = effPCPWhirlwindPolyCreate(&work->param, NULL);
    dds3WhirlwindPolyListCopyTex(child->hPoly, work->hPoly);
    return child;
}
