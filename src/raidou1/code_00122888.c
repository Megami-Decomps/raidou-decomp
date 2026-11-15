#include "common.h"

#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "newdata.h"
#include "dds/fr_font.h"
#include "dds/fr_font_context.h"
#include "dds/scr_integer_parameter_api.h"
#include "dds/scr_integer_return_value_api.h"
#include "dds/sdf_chip.h"

/* Declared void for the callers that ignore its result: ee-gcc 2.96
 * allocates registers around the call as retail did only then. The
 * definition (scrTraceCode.c) returns the context it set the value in, for
 * the script commands that pass it on as their result; their units declare
 * it so. */
void scrSetIntReturnValue(ScrVmInteger retVal);

#include "dds/common.h"
#include "dds/scr_command_parameter_index.h"
#include "dds/dat_state.h"
#include "dds/itf.h"
#include "dds/dds3_owned_node.h"
#include "dds/pty_roster_api.h"
#include "dds/pty_remove_unit_item_disposition_api.h"
#include "dds/pty_active_roster_index_api.h"
#include "dds/pty_template_init_flags.h"

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
