#include "common.h"

/* Declarations from dds-decomp src/dds2/game/code_0011EC90.c */
#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "dds/common.h"

#include "dds/scr_integer_parameter_api.h"

#include "dds/scr_command_parameter_index.h"

#include "dds/scr_integer_return_value_api.h"

#include "dds/sdf_chip.h"

#include "dds/dat_state.h"

#include "dds/itf.h"

#include "dds/fr_font.h"

#include "dds/dds3_owned_node.h"

#include "dds/pty_roster_api.h"

#include "dds/pty_remove_unit_item_disposition_api.h"

#include "dds/pty_active_roster_index_api.h"

#include "dds/pty_template_init_flags.h"

extern s32 mtrHasEnoughOwnedMantras(void);

extern PtyAddUnitResult ptyAddUnitWithFlags(DatUnitId unitId, PtyTemplateInitFlags initFlags);

extern s32 scrSetFlag(datUnitWork_t *work, u16 index);

extern void scrClearAllSecondaryScriptFlags(datUnitWork_t *work);

extern void scrSetSecondaryScriptFlag(datUnitWork_t *work, u16 index);

s32 ptyScriptRemoveUnitAndReturnResult(void);

/* Evaluate a two-operand VM expression and publish its result. */
s32 ptyScriptAddUnitWithFlagsAndReturnResult(void);

s32 ptyScriptRemoveUnitWithFlagsAndReturnResult(void);

s32 scrCmdHasEnoughOwnedMantras(void);

s32 scrCmdSetEntryFlagsInBothStores(void);

/* Append an intrusive node; linkOffset selects its previous/next pair. */
void dds3FontReqListInsert(Dds3IntrusiveNodeList *list,
                             Dds3IntrusiveNode *node, s32 linkOffset);

void dds3FontReqListRemove(Dds3IntrusiveNodeList *list,
                            Dds3IntrusiveNode *node, s32 linkOffset);

void dds3FontReqListAdd(Dds3IntrusiveNode *node,
                                    const Dds3IntrusiveNodeCallbacks *vtable);

void dds3FontReqListRunOne(Dds3IntrusiveNode *node);

void dds3FontReqListRunAll(void);

void dds3SetLinkedNodeValue(Dds3IntrusiveNode *node, u32 nodeValue);

void dds3FontReqListRunTo(s32 nodeValue);

void dds3FontReqListCount(void);

void dds3FontReqDestroy(Dds3IntrusiveNode *owner);

void dds3FontReqDrawFont(const Dds3IntrusiveNode *owner);

Dds3FontNode *dds3CreateFontNode(s32 x, s32 y, const char *text);

void dds3FontReqSetColorNo(Dds3FontNode *node, u8 value);

void dds3FontReqFree(Dds3IntrusiveNode *owner);

extern Dds3IntrusiveNodeCallbacks dds3FontNodeVTable __attribute__((section(".sdata")));
/* End of declarations from src/dds2/game/code_0011EC90.c */

s32 scrCmdHasEnoughOwnedMantras(void) {
    scrSetIntReturnValue(mtrHasEnoughOwnedMantras());
    return 1;
}
