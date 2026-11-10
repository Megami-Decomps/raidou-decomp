#include "shared/effect/effEvent/decls.h"

/* Install the staggered-blur resource-table texture without changing its slots. */
void Eve_RippleBlurSetDefTex(void) {
    hEve_RippleBlur->texture = dds3BillboardGetGeneralTex(EFF_EVENT_STAGGERED_RESOURCE_INDEX);
}
