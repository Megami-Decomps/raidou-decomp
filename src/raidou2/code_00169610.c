#include "common.h"
#include "fld.h"
#include "sdfMemory.h"
#include "code_0015D490.h"
#include "fldFileResolver.h"

INCLUDE_ASM(const s32, "code_00169610", mapGenbaPosEffStart);

INCLUDE_ASM(const s32, "code_00169610", mapGenbaPosEffAllEnd);

INCLUDE_ASM(const s32, "code_00169610", mapGenbaPosEffDraw);

void mapProcSequenceSub8(void) {
    mapIsFldKeyFreeSub4Data = NULL;
    mapIsFldKeyFreeSub4Data2 = NULL;
}

INCLUDE_ASM(const s32, "code_00169610", mapProcRandomEncountCheckFldSub);

void mapIsFldKeyFreeSub4(void) {
    if (mapIsFldKeyFreeSub4Data != 0) {
        mapIsFldKeyFreeSub4Data2--;
        if (mapIsFldKeyFreeSub4Data2 < 0) {
            mapIsFldKeyFreeSub4Data = 0;
        } else {
            fldDrawFrushOt(128, 64, 64, (mapIsFldKeyFreeSub4Data2 << 6) / 20, 4, 97);
        }
    }
}

void mapWaitCbScreenFadeBSub(s32 mode) {
    mapData121 = mode;
}

INCLUDE_ASM(const s32, "code_00169610", mapCampBADEPL);

void mapBadEffFree(s32 mode) {
    if (mode == 0) {
        if (eMapBadEff_R != NULL) {
            dds3EffectDelete(eMapBadEff_R);
            eMapBadEff_R = NULL;
        }
        if (hMapBadEff_R != NULL) {
            sdfMemFreeDirect(hMapBadEff_R);
            hMapBadEff_R = NULL;
            mapBadEffLoadData = 0;
        }
        mapData118 = 0;
        eMapBadEff_R_BADID = -1;
    } else {
        if (eMapBadEff_N != NULL) {
            dds3EffectDelete(eMapBadEff_N);
            eMapBadEff_N = NULL;
        }
        if (hMapBadEff_N != NULL) {
            sdfMemFreeDirect(hMapBadEff_N);
            hMapBadEff_N = NULL;
            mapBadEffLoadData3 = 0;
        }
        mapData120 = 0;
        eMapBadEff_N_BADID = -1;
    }
}
