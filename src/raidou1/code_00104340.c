#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"

/* body: matched in dds @ 18d1fa88f: src/dds1/kernel/dds3StartMenu.c:6, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s32 dds3TaskNoopCallback(dds3ProcessID_t *id) {
    return 0;
}
