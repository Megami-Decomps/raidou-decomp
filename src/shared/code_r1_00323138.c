#include "common.h"
#include "kernel.h"
#include "cmpFile.h"
#include "itf.h"
#include "code_r1_00323138.h"

/* NON_MATCHING: 11 of 81 words match. Frame size and callee-saved set differ: retail hoists the high half of the 7th argument address into s3. */
#ifdef NON_MATCHING
void cmpDrawBackPattern(tagCMP_MAP_WORK *wk)
{
    s32 pos[18] = { 17, 113, 63, -6, 276, 45, -69, 209, 72, 268, 161, 332, 284, 237, 389, 82, 390, 321 };
    struct sprHandle_t *sprite;
    s32 *p;
    u32 i;

    sprite = cmpGetSpriteHnd(CMP_SPR_MAP);
    if (sprite != NULL) {
        p = pos;
        i = 0;
        do {
            s32 x = p[0];
            s32 y = p[1];

            p += 2;
            i++;
#ifdef VERSION_RAIDOU1
            itfDrawMiscSpriteList(x, y, 0, 255, sprite, 103, cmpDrawBackPatternData);
#else
            itfDrawMiscSpriteList(x, y, 0, 255, sprite, 104, cmpDrawBackPatternData);
#endif
        } while (i < 9);
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_00323138", cmpDrawBackPattern);
#endif

INCLUDE_RODATA(const s32, "code_r1_00323138", cmpDrawBackPatternTable);

