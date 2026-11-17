#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "code_00346258.h"
#include "code_r1_001CC1D0.h"
#include "jmsInit.h"
#include "gohMain.h"
#include "gohDraw.h"
#include "fclMisc.h"
#include "event.h"
#include "scr.h"
#include "dds3SceneBasic.h"
#include "code_r1_002C8188.h"
#include "rstInit.h"
#include "sdfGsMemory.h"

/* body: matched in smt3 @ 9401f79e: src/smt3/sdf/sdfGsMemory.c:340, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
sdfGsMemSlice_t *sdfGsMemGetLinkHead(void) {
    return gsMemHandleLinkHead;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/sdf/sdfGsMemory.c:344, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
sdfGsMemSlice_t *sdfGsMemGetLinkTail(void) {
    return gsMemHandleLinkTail;
}
