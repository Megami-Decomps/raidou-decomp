#include "shared/basic/dds3ObjectBase/decls.h"

void dds3ObjectBaseSetPadControl(dds3Basic_t *pObjectBasic, dds3BasicControlProc_t pFuncPadControl)
{
    dds3ObjectBaseSetPrimaryControl(pObjectBasic, pFuncPadControl);
    /* Retail dds3ObjectBaseSetPadControl and DDS1 001127A8 keep JAL plus epilogue.
     * DDS cc1probe: this consumed-callback reset is removed in 03.cse,
     * after the sibling-call decision (01.sibling / 02.jump). */
    pFuncPadControl = NULL;
}
