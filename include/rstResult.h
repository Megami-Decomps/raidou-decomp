#ifndef RSTRESULT_H
#define RSTRESULT_H

#include "common.h"
#include "kernel.h"

/* One step of a result gauge table. Partial layout. */
typedef struct rstGaugeStep_t {
    s16 parent; /* step whose position this one is relative to, or negative */
    u16 value;
    s16 x;
    s16 y;
} rstGaugeStep_t;

/* A gauge table and its step count. */
typedef struct rstGaugeTable_t {
    rstGaugeStep_t *steps;
    s32 count;
} rstGaugeTable_t;

extern rstGaugeTable_t rstData3[];

u16 rstResultCyuuseiGaugeSub(s32 a_table, s32 a_step);
s32 rstBtlEndWaitInit(dds3ProcessID_t *PID);
struct rstInitWork_t;
s32 rstCalcProcess(struct rstInitWork_t *pWork);
dds3ProcessFunc_t rstDrawProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t rstEndUpdateProcess(dds3ProcessID_t *PID);
s32 rstLvUpWaitCalcProcess(dds3ProcessID_t *PID);

#endif
