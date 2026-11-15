#include "common.h"
#include "sbtlWork.h"
#include "sdk_libc.h"
#include "fileManager.h"
#include "sdf_resource.h"
#include "sdfMemory.h"
#include "sBtlUnit.h"
#include "sBtlDvltk.h"

s32 sbtlLoadOnMemMsg(void)
{
    u32 i;

    for (i = 0; i < 20; i++) {
        if (sbtlLoadOnMemMsgWork[i] != NULL) {
            sdfMemFreeDirect(sbtlLoadOnMemMsgWork[i]);
        }
        sbtlLoadOnMemMsgWork[i] = sdfDevLoadFileEx(sbtlData17[i], &sbtlWork5[i], &sbtlWork6[i]);
    }
    return 1;
}
