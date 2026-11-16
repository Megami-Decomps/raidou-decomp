#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"

extern s32 fldbokasi;

/* body: matched in smt3 @ 9401f79e: src/smt3/event/evtSky.c:49, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void evtSetBokasi(s32 sw)
{
    fldbokasi = sw;
}
