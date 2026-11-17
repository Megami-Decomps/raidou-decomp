#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "code_r1_001CC1D0.h"
#include "fclMisc.h"
#include "code_00346258.h"
#include "jmsInit.h"
#include "gohMain.h"
#include "gohDraw.h"
#include "sdfSound.h"

void jmsCallPlaySE(s32 unused, s32 inputFlags, u32 *stateFlags) {
    if (inputFlags & 0x8000) {
        p2sdPlaySE(13, 127, 63);
        return;
    }
    if (inputFlags & 0x4000) {
        p2sdPlaySE(13, 127, 63);
        return;
    }
    if (inputFlags != 0) {
        if (inputFlags & 1) {
            p2sdPlaySE(0, 127, 63);
        }
        if (inputFlags & 2) {
            p2sdPlaySE(1, 127, 63);
        }
        if (inputFlags & 0xCF0) {
            if (stateFlags != NULL) {
                if ((*stateFlags & 3) != 2) {
                    p2sdPlaySE(3, 127, 63);
                }
            } else {
                p2sdPlaySE(3, 127, 63);
            }
        }
    }
}
