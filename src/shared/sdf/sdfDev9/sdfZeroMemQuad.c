#include "shared/sdf/sdfDev9/decls.h"

/* Clear `count` quadwords at dst: four per pass, then a falling 3/2/1
 * remainder. The pass is a goto loop (no loop notes, so no loop alignment),
 * and its exit test is taken before the four stores. */
u128 *sdfZeroMemQuad(u128 *dst, s32 count) {
    u128 *p = dst;
    s32 done;
    if (count >= 4) {
loop:
        count -= 4;
        done = count < 4;
        PS2_STORE_ZERO_QUAD(p);
        PS2_STORE_ZERO_QUAD_AT16(p);
        PS2_STORE_ZERO_QUAD_AT32(p);
        PS2_STORE_ZERO_QUAD_AT48(p);
        p += 4;
        if (!done) {
            goto loop;
        }
    }
    switch (count) {
    case 3:
        PS2_STORE_ZERO_QUAD_AT32(p);
    case 2:
        PS2_STORE_ZERO_QUAD_AT16(p);
    case 1:
        PS2_STORE_ZERO_QUAD(p);
    }
    return dst;
}
