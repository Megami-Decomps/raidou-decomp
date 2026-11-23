#include "common.h"
#include "field.h"
#include "mapData.h"
#include "fld.h"
#include "dds3KernelDraw.h"
#include "dds3KernelDraw_0010A750.h"
#include "sdfAspect.h"

extern char *gMapIfMenu_Win_Work[];

void mapIfDrawActionMasterSub(s32 *x, s32 *y, s32 w)
{
    if (*x < SDF_ASPECT_LEFT + 22) {
        *x = SDF_ASPECT_LEFT + 22;
    }
    if (*x > SDF_ASPECT_RIGHT - 24 - w) {
        *x = SDF_ASPECT_RIGHT - 24 - w;
    }
}

INCLUDE_ASM(const s32, "code_00176B00", mapCallSetOt9);

INCLUDE_ASM(const s32, "code_00176B00", mapCallLinkAlpha);

INCLUDE_ASM(const s32, "code_00176B00", mapCallSetOt10);

INCLUDE_ASM(const s32, "code_00176B00", mapCallSetOt11);

INCLUDE_ASM(const s32, "code_00176B00", mapCallBitCheck8);

INCLUDE_ASM(const s32, "code_00176B00", mapRefDDS3GBWK7);

INCLUDE_ASM(const s32, "code_00176B00", mapCallSetActionWndLines);

INCLUDE_ASM(const s32, "code_00176B00", mapIfDrawAction);

INCLUDE_ASM(const s32, "code_00176B00", mapCallSetActionWndLines2);

INCLUDE_ASM(const s32, "code_00176B00", mapCallSetActionWndLines3);

s32 mapProcShutDownSub2(void)
{
    if (fldGlobalBase.amSBlock != 16 || fldGlobalBase.nameU != 0) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_00176B00", mapBUNRIHAJIKI);

INCLUDE_ASM(const s32, "code_00176B00", saveCallSetOt);

void saveIsFldKeyFreeSub(void)
{
    saveCallSetOt(mapUnloadRequestSub(), 1);
}

INCLUDE_ASM(const s32, "code_00176B00", saveFunc);

INCLUDE_ASM(const s32, "code_00176B00", saveProcSequenceSub);

INCLUDE_ASM(const s32, "code_00176B00", savePointMenuMoverFunc);

char *mapIfGetActionName(s32 mapActionID)
{
    if (mapActionID < 0) {
        return NULL;
    }
    return gMapIfMenu_Win_Work[mapActionID];
}

INCLUDE_ASM(const s32, "code_00176B00", itfIfDrawActionSub);

void UnloadRequest(s32 mode)
{
    struct mapUnloadRequestSubWork_t *work = mapUnloadRequestSub();

    if (mode == 0) {
        work->fieldC8 = 0;
        work->fieldCC = 0x80;
    } else if (mode == -1) {
        work->fieldC8 = mode;
    } else {
        work->fieldC8 = 1;
    }
}

void fldCallSetOt2(void)
{
    struct mapUnloadRequestSubWork_t *work = mapUnloadRequestSub();

    if (work->fieldC8 == -1) {
        work->fieldCC += 5;
        if (work->fieldCC > 80) {
            work->fieldCC = 80;
        }
    } else if (work->fieldC8 == 1) {
        work->fieldCC -= 5;
        if (work->fieldCC < 0) {
            work->fieldCC = 0;
            work->fieldC8 = 0;
        }
    }
    if (nbData14[0] == 0 && work->fieldC8 != 0) {
        eglSetOt(0x68);
        eglLinkAlpha(0);
        eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        evtSubmitDefaultDepthGradientRect(0, 0, 512, 448, work->fieldCC << 24, work->fieldCC << 24, work->fieldCC << 24,
                                          work->fieldCC << 24);
    }
}
