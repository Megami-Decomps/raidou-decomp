#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "sdfRand.h"
#include "dds3Randomize.h"

void dds3InitRandCB(void);

void dds3InitRandCB(void) {
    sdfRandomize(dds3RandCB, 0x12345678);
}
