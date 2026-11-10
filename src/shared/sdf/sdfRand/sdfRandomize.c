#include "shared/sdf/sdfRand/decls.h"

/* Seed the generator state (the shared effect state when `state` is NULL); each word is the previous one rotated right 8 bits and xored with a constant. */
void sdfRandomize(sdfRandCB_t *aCB, u32 aSeed) {
    if (aCB == NULL) {
        aCB = &sdfDefaultRandCB;
    }
    aSeed ^= 0x0AED1A0C;
    aCB->v[0] = aSeed;
    aSeed = (aSeed << 24) | (aSeed >> 8);
    aCB->v[1] = aSeed;
    aSeed ^= 0xAA5A02FE;
    aSeed = (aSeed << 24) | (aSeed >> 8);
    aCB->v[2] = aSeed;
    aSeed ^= 0x11BE81C7;
    aSeed = (aSeed << 24) | (aSeed >> 8);
    aCB->v[3] = aSeed;
}
