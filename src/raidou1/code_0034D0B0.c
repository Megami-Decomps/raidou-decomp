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

INCLUDE_ASM(const s32, "code_0034D0B0", jmsLeaf19);

INCLUDE_ASM(const s32, "code_0034D0B0", jmsLeaf20);

INCLUDE_ASM(const s32, "code_0034D0B0", jmsLeaf21);

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_0027BF00.c:131, found, same function 1.00 (words 13/13, calls 1.00, shape 1.00) */
void mnuResetListNodeFadeCounters(struct mnuList_t *list) {
    struct mnuListNode_t *node;

    node = list->first;
    if (node != 0) {
        node->animationTimer = 0;
        while (node = node->next, node != 0) {
            node->animationTimer = 0;
        }
    }
}
