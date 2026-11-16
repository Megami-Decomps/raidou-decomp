#ifndef EFFQUAKE_H
#define EFFQUAKE_H

#include "common.h"

#define EFFSTAT_QUAKEMAX 32

/* Quake request: 32 of these at quake_list, each forwarded to dds3SetQuake.
 * type 0 shakes until stopped, type 1 for frame frames. */
typedef struct effQuakeRequest_t {
    s32 frame;
    s32 amp;
    u16 flags;
    u16 type;
    u16 count;
    u16 priority;
} effQuakeRequest_t;

typedef char effQuakeRequestSizeCheck[(sizeof(effQuakeRequest_t) == 0x10) ? 1 : -1];

void effInit(void);
void effUpdate(void);
effQuakeRequest_t *effRegist(void);
void effStart(effQuakeRequest_t *req, s32 amp);
void effStartDamp(effQuakeRequest_t *req, s32 amp, s32 frame);
void effStartSignal(effQuakeRequest_t *req);
void effStop(effQuakeRequest_t *req);
void effRemove(effQuakeRequest_t *req);

extern effQuakeRequest_t quake_list[EFFSTAT_QUAKEMAX];
extern u32 flags_quake;

#endif
