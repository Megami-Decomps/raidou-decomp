#include "common.h"
#include "effManager.h"
#include "field.h"

extern effHandle_t *mapData37[16];
extern s32 mapWork7[16];
extern f32 mapWork8[16][4];
extern s32 mapWork9[16];
extern s32 mapWork10[16];
extern effHandle_t *cmpPanelEffList2;

extern s32 mapBigClipChkF(s32 cut, f32 x, f32 y, f32 z);

void mapCallEffectDelete(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (mapData37[i] != NULL) {
            dds3EffectDelete(mapData37[i]);
            mapWork7[i] = 0;
            mapWork8[i][0] = 0;
            mapData37[i] = NULL;
            mapWork8[i][1] = 0;
            mapWork8[i][2] = 0;
            mapWork8[i][3] = 0;
            mapWork9[i] = 0;
            mapWork10[i] = 0;
        }
    }
    if (cmpPanelEffList2 != NULL) {
        dds3EffectDelete(cmpPanelEffList2);
        cmpPanelEffList2 = NULL;
    }
}

INCLUDE_ASM(const s32, "code_r1_0015CD00", mapGenbaPosEffAdd);

/* NON_MATCHING: 18 of 43 words match. Retail tests the 64-bit result with movn and lays out the loop differently. */
#ifdef NON_MATCHING
void mapGenbaPosEffClipSet(void) {
    s32 i;
    f32 *pos = mapWork8[0];
    effHandle_t **eff = mapData37;
    s32 *clip = mapWork7;

    for (i = 0; i < 16; i++, pos += 4, clip++) {
        if (*eff++ != NULL) {
            *clip = mapBigClipChkF(fldGlobalBase.cutIdx, pos[0], pos[1], pos[2]) != 0;
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_0015CD00", mapGenbaPosEffClipSet);
#endif
