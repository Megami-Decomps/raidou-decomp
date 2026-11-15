#include "common.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "newdata.h"
#include "sdk_libc.h"
#include "dds/dds3_process.h"
#include "dds/fr_font.h"

u32 dds3GetProcessData(const dds3ProcessID_t *task);

#include "dds/sdk_ps2.h"
#include "dds/fr_font_draw_glyph.h"
#include "dds/mnu_input.h"
#include "dds/itf_draw_grid.h"
#include "dds/mnu.h"
#include "dds/mnu_staff.h"
#include "dds/mnu_list.h"
#include "dds/mnu_shop.h"
#include "dds/mnu_camp_work.h"
#include "dds/dat_state.h"
#include "dds/eff.h"

typedef struct _FRQ _FRQ;

extern s32 checkConfigState(void);
extern MenuPopupEntry mnuCampCommandReturnPopupEntry[];
s32 mnuDispatchStaffMenuWithIdlePopup(dds3ProcessID_t *callback);

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
