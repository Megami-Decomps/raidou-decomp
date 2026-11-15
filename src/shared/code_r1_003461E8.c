#include "common.h"
#include "dds3KernelCore.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "newdata.h"
#include "sdk_libc.h"
#include "dds/dds3_process.h"


#include "dds/mnu.h"

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
