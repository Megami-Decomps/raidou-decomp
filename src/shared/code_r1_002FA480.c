#include "common.h"
#include "sdk_libc.h"

extern const char fldStrFontNameEntFnt[];
extern const char *fldFontNameEntData[2];
extern void frCallDevLoadFileEx(s32 slot, const char *path);
extern void frCallMemFreeDirect(s32 slot);
extern void nbRefDDS3GBWK12(s32 idx, const char *str, s32 len, s32 slot);

void fldFontNameEnt(void) {
    s32 i;

    frCallDevLoadFileEx(4, fldStrFontNameEntFnt);
    for (i = 0; i < 2; i++) {
        nbRefDDS3GBWK12(i, fldFontNameEntData[i], strlen(fldFontNameEntData[i]) >> 1, 4);
    }
    frCallMemFreeDirect(4);
}
INCLUDE_SDATA(const s32, "code_r1_002FA480", fldFontNameEntData);

INCLUDE_SDATA(const s32, "code_r1_002FA480", fldFontNameEntDataEntry);

INCLUDE_SDATA(const s32, "code_r1_002FA480", fldFunc23DataEntry);

INCLUDE_SDATA(const s32, "code_r1_002FA480", fldFunc23DataEntry2);

INCLUDE_SDATA(const s32, "code_r1_002FA480", fldFunc23DataEntry3);

INCLUDE_SDATA(const s32, "code_r1_002FA480", fldFunc23DataEntry4);

INCLUDE_SDATA(const s32, "code_r1_002FA480", fldFunc23DataEntry5);

INCLUDE_SDATA(const s32, "code_r1_002FA480", fclFunc8Data2Entry);

INCLUDE_SDATA(const s32, "code_r1_002FA480", fclFunc8Data2Entry2);

INCLUDE_SDATA(const s32, "code_r1_002FA480", fclFunc8Data2Entry3);

