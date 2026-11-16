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

typedef struct _FRQ _FRQ;

extern s32 checkConfigState(void);
extern jmsSeqUnit_t mnuCampCommandReturnPopupEntry[];
s32 mnuDispatchStaffMenuWithIdlePopup(dds3ProcessID_t *callback);

/* Dispatch a callback; on idle, install the default entry unless busy. */
s32 mnuDispatchStaffMenuWithIdlePopup(dds3ProcessID_t *callback) {
    jmsSeqWork_t *work = dds3GetProcessData(callback);
    s32 state = (s32)menuRunPanel(&work->panel, &work->seqUnit, 0, callback);
    if (state == 0) {
        if (checkConfigState() == 0) {
            jmsSeqJumpAndFree(&work->seqUnit, mnuCampCommandReturnPopupEntry);
        }
        return 0;
    }
    return state;
}
