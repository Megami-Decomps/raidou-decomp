#include "shared/effect/effEvent/decls.h"

/* Install the textured-square resource-table texture while retaining its body. */
void Eve_PartialMultiFilterSetDefTex(void) {
    hEve_PartialMultiFilter->hAlphaTex = dds3BillboardGetGeneralTex(EFF_EVENT_SQUARE_RESOURCE_INDEX);
}
