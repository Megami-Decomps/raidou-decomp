#include "shared/kernel/dds3KernelCore/decls.h"

/* namehash is the name's byte sum, which dds3SearchProcessName uses to reject
 * most candidates without a compare. */
dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2, s32 v3,
                                  dds3ProcessFunc_t drawExit, dds3ProcessFunc_t deleteExit,
                                  void *p)
{
    dds3ProcessID_t *setp = sdfChipAllocDirect(sizeof(dds3ProcessID_t));
    s32 i = 0;

    if (setp == NULL) {
        return NULL;
    }
    setp->namehash = 0;
    while ((setp->name[i] = v1[i]) != 0 && i < 0x18) {
        setp->namehash += v1[i];
        i++;
    }
    setp->priority = dDS3_PRIORITY_DRAW_PB;
    setp->flag = 1;
    setp->starttimer = SDF_FRAMES(v2);
    setp->shutdowntimer = SDF_FRAMES_COUNTDOWN(v3);
    setp->processfunc = drawExit;
    setp->shutdownfunc = deleteExit;
    setp->workdata = p;
    setp->name[0x17] = 0;
    setp->starttime = 0;
    setp->runtime = 0;
    setp->next = NULL;
    setp->prev = NULL;
    setp->parent = NULL;
    setp->child = NULL;
    setp->sibling = NULL;
    AddProcessList(setp);
    if (setp->starttimer == 0) {
        StartProcess(setp);
    }
    return setp;
}
