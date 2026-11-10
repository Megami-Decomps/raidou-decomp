#include "shared/kernel/dds3Randomize/decls.h"

void dds3InitRandCB(void) {
    sdfRandomize(dds3RandCB, 0x12345678);
}
