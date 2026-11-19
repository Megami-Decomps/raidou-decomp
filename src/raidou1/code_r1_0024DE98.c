#include "common.h"
#include "dds3Basic.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "dds3CameraObjectBasic.h"
#include "dds3ObjectBase.h"
#include "sbtlWork.h"
#include "sdk_libc.h"
#include "ps2asm.h"
#include "dds3KernelCore.h"
#include "fclMisc.h"
#include "rstInit.h"
#include "sBtlUnit.h"
#include "fld.h"
#include "sBtlScene.h"

/* NON_MATCHING: 28 of 53 words match. Register choice (a0 vs v1) and load order of the three globals differ; logic as retail. */
#ifdef NON_MATCHING
s32 sbtlSceneLoad(void)
{
    s32 map[2];
    s32 first = sbtlSceneLoadData;

    if (first == 0 && sbtlSceneLoadData2 == 0) {
        wk_sbtl->mapNo[0] = sbtlData100[wk_sbtl->stageNo].mapNo[0];
        wk_sbtl->mapNo[1] = sbtlData100[wk_sbtl->stageNo].mapNo[1];
    } else {
        wk_sbtl->mapNo[1] = sbtlSceneLoadData2;
        wk_sbtl->mapNo[0] = first;
        sbtlSceneLoadData = 0;
        sbtlSceneLoadData2 = 0;
    }
    if (sbtlSceneLoadData3 != 0) {
        sbtlSceneLoadData3 = 0;
        sbtlRndEncTopPriorityMap(&map[0], &map[1]);
        if (map[0] > 0) {
            wk_sbtl->mapNo[0] = map[0];
            wk_sbtl->mapNo[1] = map[1];
        }
    }
    fclFilterOFF();
    dds3StartProcess("sbtlscene_load", 1015, 1, 1, DataLoadProc, DataLoadShutdown, NULL);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024DE98", sbtlSceneLoad);
#endif
