#include "common.h"
#include "kernel.h"
#include "dds3KernelCore.h"
#include "include_asm.h"
#include "jmsMisc.h"
#include "TitleMain.h"

/* NON_MATCHING: 64 of 128 words match. Retail copies the 8-byte size constant with ldl/ldr before the tables; mine loads two floats. Unsized extern avoids gp addressing. */
#ifdef NON_MATCHING
extern const titleKeyTable_t evtFunc11Table;
extern const titleKeyTable_t evtFunc11Table2;
extern const f32 evtFunc11Data[];

void evtFunc11(titleCursor_t *cursor, void **sprite)
{
    f32 size[2] = { evtFunc11Data[0], evtFunc11Data[1] };
    f32 scale[4];
    titleKeyTable_t keys0 = evtFunc11Table;
    titleKeyTable_t keys1 = evtFunc11Table2;
    s32 i;

    scale[0] = (f32)(6 - cursor->ScaleTimer) * 0.5f / 6.0f + 0.5f;
    scale[1] = 1.0f;
    evtLeaf31(cursor->Index + 5, evtFunc9(cursor->Timer, keys0.key, 3), size, scale, sprite);
    cursor->Timer++;
    if (cursor->Timer > keys0.key[2].frame) {
        cursor->Timer = 0;
    }
    evtFunc10(9, 256, sprite);
    if (cursor->Index == 3) {
        evtFunc10(10, 256, sprite);
    }
    for (i = 0; i < 5; i++) {
        evtFunc10(i + 11, evtFunc9(cursor->Timer2, keys1.key, 3) * cursor->ItemAlpha[i] / 128, sprite);
    }
    cursor->Timer2++;
    if (cursor->Timer2 > keys1.key[2].frame) {
        cursor->Timer2 = 0;
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_002F59D0", evtFunc11);
#endif
