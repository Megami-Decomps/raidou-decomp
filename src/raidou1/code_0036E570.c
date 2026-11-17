#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "sdf_resource.h"
#include "sdfTex.h"
#include "model.h"
#include "sdfModel.h"
#include "event.h"
#include "scr.h"
#include "dds3SceneBasic.h"
#include "sdf2D.h"

/* body: matched in smt3 @ 9401f79eb: src/smt3/sdf/sdfDev9.c:178, found, same function 1.00 (words 19/19, calls 1.00, shape 1.00) */
s32 sdfLeaf29(s32 number) {
    s32 digitShift = 0;
    s32 packedDigits = 0;
    while (number > 0) {
        s32 quotient = number / 10;
        packedDigits |= (number - quotient * 10) << digitShift;
        number = quotient;
        digitShift += 4;
    }
    return packedDigits;
}
