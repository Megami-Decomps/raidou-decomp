#include "common.h"

/* Declarations from smt3-decomp src/smt3/kernel/dds3Randomize.c */
#include "common.h"

#include "kernel.h"

#include "libsdf.h"

extern u8 dds3RandCB[];

extern void sdfRandomize(void *aCB, u32 aSeed);

void dds3InitRandCB(void);
/* End of declarations from src/smt3/kernel/dds3Randomize.c */

void dds3InitRandCB(void) {
    sdfRandomize(dds3RandCB, 0x12345678);
}
