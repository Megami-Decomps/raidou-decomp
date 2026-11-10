#include "shared/facility/fclMisc/decls.h"

void shuffleIndices(s8 *order, s32 count, s32 rounds)
{
    s32 n;

    if (rounds <= 0) {
        return;
    }
    n = rounds;
    while (n != 0) {
        s32 first = (u32)dds3GetRandA(0) % (u32)count;
        s32 second = (u32)dds3GetRandA(0) % (u32)count;
        u8 value = order[first];

        order[first] = order[second];
        order[second] = value;
        n--;
    }
}
