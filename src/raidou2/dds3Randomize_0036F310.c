#include "common.h"
#include "dds3Randomize.h"

u32 dds3GetRandA(void *state)
{
    sdfRandCB_t *cb = state;
    u32 x0;
    u32 x1;
    u32 x2;
    u32 x3;
    u32 rand;

    if (cb == NULL) {
        cb = &sdfDefaultRandCB;
    }
    x0 = cb->v[0];
    x1 = cb->v[1];
    x2 = cb->v[2];
    x3 = cb->v[3];
    /* Mix the high two bits of x0 and high bit of x2 into the shifted taps. */
    rand = ((x1 << 2) | ((x0 >> 30) % 4)) ^ ((x3 << 1) | ((x2 >> 31) % 2));
    cb->v[0] = rand;
    cb->v[1] = x0;
    cb->v[2] = x1;
    cb->v[3] = x2;
    return rand;
}

f32 dds3GetRandFloatA(void *state) {
    u32 value = dds3GetRandA(state) & 0xFFFFFF;
    return (f32)value * 5.9604644775390625e-8f;
}

u32 dds3GetRandIntA(void *aLimit, u32 name)
{
    return dds3GetRandA(aLimit) % name;
}
