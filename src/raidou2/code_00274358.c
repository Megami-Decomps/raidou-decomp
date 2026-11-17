#include "common.h"
#include "sbtlWork.h"
#include "sBtlUnit.h"

/* Raidou 1 calls this as the out-of-line stub sbtlLeaf10, which returns NULL. */
static inline u32 *sbtlGetActionFlags(void)
{
    return NULL;
}

s32 sbtlFunc2(struct SBTLUNIT_WORK *wk, s32 type)
{
    SBTL_DEVIL_DATA *devil = sbtlIfDrawActionSub(wk->unit.devilId);
    u32 *flags;

    sbtlGetMotionVisualData(wk->major, wk->minor);
    wk->flg &= ~0x4000;
    switch (type) {
    case 0:
        wk->flg |= devil->flag;
        break;
    case 1:
        flags = sbtlGetActionFlags();
        /* BUG: the stub returns NULL, so this reads address 4. */
#ifdef FIX_BUGS
        if (flags == NULL) {
            break;
        }
#endif
        wk->flg |= flags[1];
        break;
    }
    return 1;
}
