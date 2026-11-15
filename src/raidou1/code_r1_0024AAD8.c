#include "common.h"
#include "sBtlScript.h"
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
#include "dds3KernelMain.h"
#include "sBtlScene.h"

s32 sbtlSbattle(SBTL_START_DATA *start, s8 flag)
{
    dds3ProcessID_t *process;

    sbtlSbattleData = dds3GetKernelCount();
    main_data = *start;
    if (flag == 0) {
        sbtlCallBtlDeleteOnData();
        rstSetFlag(0x20000);
    }
    sbtlCallMemset3(start->val30, start->val28);
    process = dds3StartProcess(sbtlData85, 1015, 0, 0, sbtlSbattle5, sbtlLeaf, NULL);
    wk_sbtl->process = process;
    dds3LinkProcess(process, dds3StartProcess("sbattle_draw", 11020, 0, 0, DrawProc, NULL, NULL));
    dds3LinkProcess(process, dds3StartProcess("sbattle_draw2", 11021, 0, 0, sbtlSbattle6, NULL, NULL));
    sbtlSceneLoad();
    Init_sbtlUnit(flag);
    sbtlCreateAIScript();
    sbtlInitDropWork();
    RestartBattle();
    sbtlStopVibe();
    return 1;
}
