#include "common.h"
#include "dds3KernelCore.h"

#include "newdata.h"
#include "sdk_libc.h"
#include "include_asm.h"
#include "kernel.h"
#include "draw3d.h"
#include "basic.h"
#include "evt_world.h"
#include "code_r1_001CC1D0.h"

static inline s32 menuRunPanel(void *context, s32 mode, void *callback) {
    return jmsSeqChange((MenuPopupState *)((u8 *)context + 8),
                                      (s32 *)((u8 *)context + 0x54), mode,
                                      callback);
}

typedef struct _FRQ _FRQ;

extern s32 checkConfigState(void);
extern MenuPopupEntry mnuCampCommandReturnPopupEntry[];
s32 mnuDispatchStaffMenuWithIdlePopup(dds3ProcessID_t *callback);

/* Dispatch a callback; on idle, install the default entry unless busy. */
s32 mnuDispatchStaffMenuWithIdlePopup(dds3ProcessID_t *callback) {
    s32 context = EE_ADDR_S(dds3GetProcessData(callback));
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
