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

void evtFunc11(titleLogoState_t *state, void **sprite)
{
    f32 size[2] = { evtFunc11Data[0], evtFunc11Data[1] };
    f32 scale[4];
    titleKeyTable_t keys0 = evtFunc11Table;
    titleKeyTable_t keys1 = evtFunc11Table2;
    s32 i;

    evtLeaf31(state->phase + 5, evtFunc9(state->frame0, keys0.key, 3), size, scale, sprite);
    scale[0] = (f32)(6 - state->step) * 0.5f / 6.0f + 0.5f;
    scale[1] = 1.0f;
    state->frame0++;
    if (state->frame0 > keys0.key[2].frame) {
        state->frame0 = 0;
    }
    evtFunc10(9, 256, sprite);
    if (state->phase == 3) {
        evtFunc10(10, 256, sprite);
    }
    for (i = 0; i < 5; i++) {
        evtFunc10(i + 11, evtFunc9(state->frame1, keys1.key, 3) * state->weight[i] / 128, sprite);
    }
    state->frame1++;
    if (state->frame1 > keys1.key[2].frame) {
        state->frame1 = 0;
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_002F59D0", evtFunc11);
#endif
