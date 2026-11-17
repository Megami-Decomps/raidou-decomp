#include "common.h"
#include "basic.h"
#include "dds3PathBasic.h"

void dds3PathBasicPauseOn(dds3PathBasicControl_t *pControl)
{
    pControl->flag |= DDS3_PATHBASIC_FLAG_STOP;
}

void dds3PathBasicPauseOff(dds3PathBasicControl_t *pControl)
{
    pControl->flag &= ~DDS3_PATHBASIC_FLAG_STOP;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3PathBasic.c:299, match, same function 1.00 (words 4/4, calls 1.00, shape 1.00) */
void dds3PathBasicLoopOn(dds3PathBasicControl_t *pControl)
{
    pControl->flag |= DDS3_PATHBASIC_FLAG_LOOP;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3PathBasic.c:304, match, same function 1.00 (words 5/5, calls 1.00, shape 1.00) */
void dds3PathBasicLoopOff(dds3PathBasicControl_t *pControl)
{
    pControl->flag &= ~DDS3_PATHBASIC_FLAG_LOOP;
}
