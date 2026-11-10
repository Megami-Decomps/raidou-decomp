#include "shared/effect/effEvent/decls.h"

/* Replace the setup body without replacing the active textured-square work. */
void mfilterSetParam(dds3Effect_PartialMultiFilterParam_t *parameters) {
    mfilterParam = *parameters;
}
