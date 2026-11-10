#ifndef DDS_SCR_H

#define DDS_SCR_H
#include "../scr.h"
/* The FLW0 script interpreter: the loaded file's tables, and the per-procedure
 * work block scrTraceCode steps through.
 * See include/README.md for how the layouts were verified. */

#include "common.h"

#include "kwln.h"

#include "scr_trace_result.h"

#include "scr_command_result.h"

#include "scr_command_index.h"

#include "scr_procedure_index.h"

#include "scr_command_parameter_index.h"

#include "scr_command_parameter_count.h"

#include "scr_command_yield_count.h"

#include "scr_vm_integer.h"

#include "itf_mes_window_index.h"

/* DDS1/2 additions with no SMT3 counterpart in this header. */
struct ItfMesSub;

#define SCR_STACK_RETURN_VALUE_SLOT 27

#endif /* DDS_SCR_H */
