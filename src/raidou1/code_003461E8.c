#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_00272798.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "newdata.h"

#include "sdk_libc.h"

#include "dds/fr_font.h"

#include "dds/kwln.h"

u32 dds3GetProcessData(const dds3ProcessID_t *task);

/* Link a glyph chain into the current draw-buffer slot. */
s32 frDestroyFontRequestAfter(struct _FRQ *glyph);

/* Update the first-option byte of every child on this glyph chain. */
void frSetFontColorNo(struct _FRQ *glyph, u8 firstOption);

#include "dds/sdk_ps2.h"

#include "dds/fr_font.h"

#include "dds/fr_font_draw_glyph.h"

#include "dds/mnu_input.h"

#include "dds/itf_draw_grid.h"

#include "dds/kwln.h"

#include "dds/mnu.h"

#include "dds/mnu_staff.h"

#include "dds/mnu_list.h"

#include "dds/mnu_shop.h"

#include "dds/mnu_camp_work.h"

#include "dds/dat_state.h"

#include "dds/eff.h"

extern void dds3ConfigProcessStart(s32 mode);

typedef struct _FRQ _FRQ;

extern u32 itfCreateFont2TextGlyph(s32, s32, s32, u32, char *, s32);

extern char D_003BC6C8[];

extern const char D_003BC6D0[];

extern char D_003BC6D8[];

extern s32 checkConfigState(void);

extern MenuPopupEntry mnuCampCommandReturnPopupEntry[];

extern void mnuDrawBackdrop(s32, s32);

extern void mnuDrawStaffScrollPanel(s32, s32, s32, u32, s32);

extern void mnuCreateStaffImageSprite(s32);

extern void mnuClearWindowListSelectionFlags(MenuWindowContainer *);

extern void mnuPlayInputSound(s32, s32, u32 *);

extern MenuPopupEntry mnuCampCommandDispatchEntries[];

s32 mnuHandleCampFieldSkillInput(dds3ProcessID_t *callback);

extern void mnuDrawCampGridAndLabelDefault(s32, s32, const void *, StaffMenuWork *, s32, s32);

extern void mnuDrawStaffCampSlotsAndCurrency(s32, s32, s32, struct EffectSlotSet *, struct EffectSlotSet *, s32);

extern const u8 mnuCampCommandHelpTextBank[];

s32 mnuDrawStaffCampImageScreen(dds3ProcessID_t *task);

/* Submit a request to the active menu dispatcher in mode 2. */
s32 mnuFinishStaffConfigPopup(dds3ProcessID_t *request);

extern void mnuSetStaffDisplayMode(MnuStaffVisualCategory nextCategory, StaffMenuWork *menu);

s32 mnuStartStaffDisplay(dds3ProcessID_t *task);

/* Switch the staff display to the fourth base resource. */
u32 mnuConfigureCampDrawContextPanel(dds3ProcessID_t *task);

/* Dispatch a callback; on idle, install the default entry unless busy. */
s32 mnuDispatchStaffMenuWithIdlePopup(dds3ProcessID_t *callback);

s32 mnuDrawStaffImageScreen(dds3ProcessID_t *callback);

s32 mnuFinishStaffImagePopup(dds3ProcessID_t *request);

/* Always allow entry to the registered staff-display return popup. */
u32 mnuAllowStaffDisplayReturn(void);

/* Draw the item quantity when present, otherwise show both staff-item slots. */
void mnuDrawStaffItemListEntry(s32 x, s32 y, s32 z, struct MenuList *list,
                  struct MenuListNode *node, s32 drawArg);
/* End of declarations from src/dds1/game/code_00272798.c */

/* Dispatch a callback; on idle, install the default entry unless busy. */
s32 mnuDispatchStaffMenuWithIdlePopup(dds3ProcessID_t *callback) {
    s32 context = dds3GetProcessData(callback);
    s32 *dispatchEntry = (s32 *)(context + 0x54);
    s32 state = menuRunPanel((void *)context, 0, (void *)callback);
    if (state == 0) {
        if (checkConfigState() == 0) {
            jmsSeqJumpAndFree(dispatchEntry, mnuCampCommandReturnPopupEntry);
        }
        return 0;
    }
    return state;
}
