#ifndef JMSMISC_H
#define JMSMISC_H

#include "common.h"
#include "code_r1_001CC1D0.h"
#include "newdata.h"

struct jmsSeqHandle_t;
struct jmsSeqUnit_t;

/* Negative while the trigger MskL2Task waits for is pressed (HD CHECK_TRIG(8201, 1)). */
extern s8 jmsChkCommonInputData[];
extern s8 jmsChkCommonInputData2[];

#ifdef VERSION_RAIDOU2
void jmsCallMemset(jmsMemsetWork_t *work);
#endif

u32 jmsChkCommonInput(u32 ControlBit);
s32 jmsChkSeqChange(struct jmsSeqUnit_t *pSeqUnit);
s32 jmsSeqChange(jmsSeqHandle_t *pSeqHandle, s32 *pSeqUnit, s32 Mode, dds3ProcessID_t *ID);
s32 jmsSeqChkLast(struct jmsSeqHandle_t *pSeqHandle, struct jmsSeqUnit_t *pSeqUnit);
void mnuClearPanelTransitionState(jmsSeqHandle_t *state);
void jmsSeqJumpAndPop(struct jmsSeqUnit_t **pCurrentSeqUnit, struct jmsSeqUnit_t *pSeqUnit);

#endif
