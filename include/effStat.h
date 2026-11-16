#ifndef EFFSTAT_H
#define EFFSTAT_H

#include "common.h"
#include "kernel.h"

void Init_effStat(void);
s32 Process_effStat(dds3ProcessID_t *proc);
void effUpdateBlur(void);
void effFadeInBlur(void);
void effCallRestoreStat(void);
void fclBtlDebugSetStatusflgSub(void);

/* eff_stat process state: stop flags, the process, and the blur fade
 * (flags and per-frame step). */
extern u32 flags_stat;
extern dds3ProcessID_t *proc_stat;
extern u32 flags_blur;
extern f32 vel_blur;
extern s32 effModelUpdateControlFlags;
extern const char Init_effStatStrEffStat[];

#endif
