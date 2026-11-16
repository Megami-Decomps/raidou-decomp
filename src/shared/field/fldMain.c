#include "common.h"
#include "fldMain.h"
#include "basic.h"
#include "dds3Basic.h"
#include "dds3CameraObjectBasic.h"
#include "dds3SceneBasic.h"
#include "dds3WorldBasic.h"
#include "sdk_libc.h"

extern const sdfVector4_t fldSetUnitFollowCameraTable;
extern const sdfVector4_t fldSetUnitFollowCameraTable2;
extern char fldCameraUnitName[];
extern dds3Basic_t *cameraBasic;

void fldSetUnitFollowCamera(void) {
    sdfVector4_t up = fldSetUnitFollowCameraTable;
    sdfVector4_t eye;
    sdfVector4_t target;

    memset(&eye, 0, sizeof(eye));
    eye.w = 1.0f;
    target = fldSetUnitFollowCameraTable2;
    cameraBasic = dds3AddCameraObjectBasicTypeLookAt(dds3AllocObjectId(), &eye, &target, &up);
    dds3SetUnitObjectId(cameraBasic, fldCameraUnitName);
    dds3CameraObjectBasicGetCameraTM(cameraBasic, &target);
    dds3GetRadiusPoseSet(cameraBasic, 2.0f);
    dds3SetActiveCameraSceneBasic(dds3GetControlScene(), cameraBasic);
}

INCLUDE_RODATA(const s32, "field/fldMain", fldSetUnitFollowCameraTable);

INCLUDE_RODATA(const s32, "field/fldMain", fldSetUnitFollowCameraTable2);

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "field/fldMain", fldStrTestReserve);

#endif
INCLUDE_RODATA(const s32, "field/fldMain", fldStrTestDarkRealmDistrict);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrTestRealWorldDistrict);

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "field/fldMain", fldStrTestOp);
#else
INCLUDE_RODATA(const s32, "field/fldMain", fldStrTestBattle);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "field/fldMain", fldStrTestMap);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrTestBattle);
#else
INCLUDE_RODATA(const s32, "field/fldMain", fldStrBattleTest);
#endif

INCLUDE_RODATA(const s32, "field/fldMain", fldStrBattleSmall);

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "field/fldMain", fldStrBattleTest);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrBattleLarge);
#else
INCLUDE_RODATA(const s32, "field/fldMain", fldStrDarkMannenChoTsukigata);
#endif

INCLUDE_RODATA(const s32, "field/fldMain", fldStrDarkReserve);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrDarkSenjuKuAmazing);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrDarkDungeonForLevelUp);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrDarkSuperfield);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrDarkSatelliteTaiItsu);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrDarkKoutouKuWadenOne);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrDarkSubterranean);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrDarkSenjuKuDaidouji);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrDarkYaraiKuDaidouji);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrDarkAkasakaKuKasumidai);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrDarkShinodaNameless);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrDarkKoutouKuSakuradayama);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrDarkKoutouKuHarumiCho);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrDarkSenjuKuFukagawaCho);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrDarkChujouKuGinzaCho);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrDarkYaraiKuTsukudoCho);

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "field/fldMain", fldStrRealReserve);
#else
INCLUDE_RODATA(const s32, "field/fldMain", fldStrTentoCaveEntrance);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrTsukigataVillage);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrMannenCho);
#endif

INCLUDE_RODATA(const s32, "field/fldMain", fldStrRealLevelUp2Nd);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrRealTutorial);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrRealSenjuKuAmazing);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrRealDungeonForLevelUp);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrRealSuperfield);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrRealSatelliteTaiItsu);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrRealKoutouKuWadenOne);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrRealSubterranean);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrRealSenjuKuDaidouji);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrRealYaraiKuDaidouji);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrRealAkasakaKuKasumidai);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrRealShinodaNameless);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrRealKoutouKuSakuradayama);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrRealKoutouKuHarumiCho);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrRealSenjuKuFukagawaCho);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrRealChujouKuGinzaCho);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrRealYaraiKuTsukudoCho);

INCLUDE_RODATA(const s32, "field/fldMain", fldStrEternalMissingNumber);

INCLUDE_SDATA(const s32, "field/fldMain", fldCameraUnitName);

#ifdef VERSION_RAIDOU2
INCLUDE_SDATA(const s32, "field/fldMain", fldData3Entry);

#endif
