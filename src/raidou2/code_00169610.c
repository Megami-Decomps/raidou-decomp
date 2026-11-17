#include "common.h"
#include "fld.h"
#include "sdfMemory.h"
#include "code_0015D490.h"

INCLUDE_ASM(const s32, "code_00169610", mapGenbaPosEffStart);

INCLUDE_ASM(const s32, "code_00169610", mapGenbaPosEffAllEnd);

INCLUDE_ASM(const s32, "code_00169610", mapGenbaPosEffDraw);

INCLUDE_ASM(const s32, "code_00169610", mapProcSequenceSub8);

INCLUDE_ASM(const s32, "code_00169610", mapProcRandomEncountCheckFldSub);

INCLUDE_ASM(const s32, "code_00169610", mapIsFldKeyFreeSub4);

INCLUDE_ASM(const s32, "code_00169610", mapWaitCbScreenFadeBSub);

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
