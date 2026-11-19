#ifndef RSTRESULT_H
#define RSTRESULT_H

#include "common.h"
#include "kernel.h"

/* One step of a result gauge table. Partial layout. */
typedef struct rstGaugeStep_t {
    s16 unk0;
    u16 value;
    s32 unk4;
} rstGaugeStep_t;

/* A gauge table and its step count. */
typedef struct rstGaugeTable_t {
    rstGaugeStep_t *steps;
    s32 count;
} rstGaugeTable_t;

extern rstGaugeTable_t rstData3[];

u16 rstResultCyuuseiGaugeSub(s32 a_table, s32 a_step);
s32 rstBtlEndWaitInit(dds3ProcessID_t *PID);
dds3ProcessFunc_t rstCalcProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t rstDrawProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t rstEndUpdateProcess(dds3ProcessID_t *PID);
s32 rstLvUpWaitCalcProcess(dds3ProcessID_t *PID);

#endif
