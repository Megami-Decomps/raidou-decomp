#include "shared/effect/effEvent/decls.h"

/* Copy the resource template body while preserving its selected resource. */
void Eve_PartialMultiFilterSetParam(dds3Effect_PartialMultiFilterParam_t *pParam) {
    hEve_PartialMultiFilter->param = *pParam;
}
