#ifndef DDS_BTL_ANALYSIS_PANEL_WAIT_ARGS_H

#define DDS_BTL_ANALYSIS_PANEL_WAIT_ARGS_H
#include "btl.h"

/* Eight-byte callback arguments allocated by btlAllocTask(8). The first word
 * is the battle unit shown by the analysis panel; the second counts callback
 * updates before panel creation. */
typedef struct BtlAnalysisPanelWaitArgs {
    BtlUnit *unit;
    u32 elapsedTicks;
} BtlAnalysisPanelWaitArgs;

typedef char BtlAnalysisPanelWaitArgs_size_must_be_0x08[
    (sizeof(BtlAnalysisPanelWaitArgs) == 0x08) ? 1 : -1];

#endif
