#ifndef CODE_002D7950_H
#define CODE_002D7950_H

#include "common.h"
#include "kernel.h"
#include "lmap.h"

/* Partial layout of the camp DH icon record. */
typedef struct campDhIcon_t {
    u32 handle;
    u8 unk04[0x10];
    u32 flags;
} campDhIcon_t;

s32 campDhIconIsReady(campDhIcon_t *pIcon);


s32 ListProcess(dds3ProcessID_t *PID);
s32 ListDrawProcess(dds3ProcessID_t *PID);
void ListProcessStart(dds3ProcessID_t *PID);
s32 getLeaf(lmapLmpListWork_t *pWork);
void getListProcessStartSub(lmapLmpListWork_t *pWork);

#endif
