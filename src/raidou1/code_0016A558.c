#include "common.h"
#include "fldProcess.h"
#include "code_0016A558.h"

void mapIsFldKeyFreeSub(void) {
    mapCallSetOt(1);
}

INCLUDE_ASM(const s32, "code_0016A558", mapLeaf2);

void mapIfMenuInit(void) {
    mapBadMafuHajikiData2 = 4;
    mapWork12.unk00 = 1;
    mapWork12.unk04 = 0;
    mapWork12.unk08 = 0;
    mapWork12.unk0C = 0;
    mapWork12.unk10 = 0;
    mapWork12.unk14 = 300;
    mapWork12.unk18 = 32;
    mapWork12.unk1C = 192;
    mapWork12.unk20 = 176;
    mapWork12.unk24 = 2;
    mapWork12.unk28 = 7;
}

INCLUDE_ASM(const s32, "code_0016A558", mapIgnite2);
