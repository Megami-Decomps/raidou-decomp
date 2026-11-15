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

/* NON_MATCHING: 43 of 157 words match. Scheduling: the vec1.w store and rodata address setup come in a different order than retail; fields after the player base are SBTL_PLAYER_DATA offsets only. */
#ifdef NON_MATCHING
s32 sbtlInitPlayerData(void)
{
    static const SBTL_PLAYER_BLOCK blockInit = { { 0x33, 0x33, 0, 0 }, { 6, 3, 6, 5 } };
    static const SBTL_PLAYER_GAUGE gaugeInit = { { 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 0, 0, 0, 0, 0, 0 } };
    sdfVector4_t vec0;
    sdfVector4_t vec1;
    SBTL_PLAYER_BLOCK block;
    SBTL_PLAYER_GAUGE gauge;
    SBTL_PLAYER_DATA *player;

    memset(&vec0, 0, sizeof(vec0));
    vec0.w = 1.0f;
    memset(&vec1, 0, sizeof(vec1));
    block = blockInit;
    vec1.w = 1.0f;
    gauge = gaugeInit;
    player = sbtlGetPlayerData();
    SDF_VU0_LOADVECTOR(&vec0);
    SDF_VU0_STOREVECTOR(&player->vec[0]);
    SDF_VU0_LOADVECTOR(&vec1);
    SDF_VU0_STOREVECTOR(&player->vec[1]);
    player->val20 = 100;
    player->val24 = 1000;
    player->val28 = 200;
    player->val2C = 9999;
    player->valF9C = 6;
    player->val3C = 1;
    player->val34 = 3;
    player->val48 = 0;
    player->block[0] = block;
    player->block[1] = block;
    player->gauge = gauge;
    player->val238 = 24;
    player->val154 = 80;
    player->val146 = 0;
    player->val14C = 0;
    player->val150 = 24;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024B510", sbtlInitPlayerData);
#endif
