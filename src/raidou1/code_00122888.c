#include "common.h"
#include "scrTraceCode.h"

#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "newdata.h"
#include "dds/scr_integer_parameter_api.h"

/* Declared void for the callers that ignore its result: ee-gcc 2.96
 * allocates registers around the call as retail did only then. The
 * definition (scrTraceCode.c) returns the context it set the value in, for
 * the script commands that pass it on as their result; their units declare
 * it so. */

extern s32 mtrHasEnoughOwnedMantras(void);
s32 ptyScriptRemoveUnitAndReturnResult(void);

/* Evaluate a two-operand VM expression and publish its result. */
s32 ptyScriptAddUnitWithFlagsAndReturnResult(void);
s32 ptyScriptRemoveUnitWithFlagsAndReturnResult(void);
s32 scrCmdHasEnoughOwnedMantras(void);
s32 scrCmdSetEntryFlagsInBothStores(void);

s32 scrCmdHasEnoughOwnedMantras(void) {
    scrSetIntReturnValue(mtrHasEnoughOwnedMantras());
    return 1;
}
