#include "common.h"
#include "fld.h"

typedef struct mapFuncEntry_t {
    u8 scene;
    u8 id;
    u8 flag;
    u8 num;
    s8 x;
    s8 y;
    s16 type;
} mapFuncEntry_t;

extern mapFuncEntry_t mapFunc10Data[19];
extern s8 mapWork13[16];
extern s32 mapCallSetOt2Data5;
extern s32 mapCallSetOt2Data6;

/* NON_MATCHING: 51 of 62 words match. Register choice for the mapWork13 base differs. */
#ifdef NON_MATCHING
void mapFunc10(void) {
    s32 i;
    s32 j;

    for (i = 15; i >= 0; i--) {
        mapWork13[i] = -1;
    }
    for (i = 0; i < 19; i++) {
        if (mapFunc10Data[i].scene != mapCallSetOt2Data5) {
            continue;
        }
        if (mapFunc10Data[i].id != mapCallSetOt2Data6) {
            continue;
        }
        if (mapRefDDS3GBWK4(mapFunc10Data[i].scene, mapFunc10Data[i].id, mapFunc10Data[i].flag)) {
            for (j = 0; j < 16; j++) {
                if (mapWork13[j] == -1) {
                    mapWork13[j] = i;
                    break;
                }
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_0016B3A0", mapFunc10);
#endif
