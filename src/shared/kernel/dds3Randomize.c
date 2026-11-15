#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "sdfRand.h"

/* Unsized so ee-gcc addresses it absolutely, not through $gp. */
extern sdfRandCB_t dds3RandCB[];
void dds3InitRandCB(void);

void dds3InitRandCB(void) {
    sdfRandomize(dds3RandCB, 0x12345678);
}
