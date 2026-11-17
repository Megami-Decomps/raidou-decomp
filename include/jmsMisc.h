#ifndef JMSMISC_H
#define JMSMISC_H

#include "common.h"
#include "code_r1_001CC1D0.h"

struct jmsSeqHandle_t;
struct jmsSeqUnit_t;

/* Negative while the trigger MskL2Task waits for is pressed (HD CHECK_TRIG(8201, 1)). */
extern s8 jmsChkCommonInputData[];

#ifdef VERSION_RAIDOU2
typedef struct jmsMemsetWork_t {
    /* 0x00 */ u8 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ u8 pad10[0x40];
} jmsMemsetWork_t;

void jmsCallMemset(jmsMemsetWork_t *work);
#endif

u32 jmsChkCommonInput(u32 ControlBit);
s32 jmsChkSeqChange(struct jmsSeqUnit_t *pSeqUnit);
s32 jmsSeqChange(jmsSeqHandle_t *pSeqHandle, s32 *pSeqUnit, s32 Mode, dds3ProcessID_t *ID);
s32 jmsSeqChkLast(struct jmsSeqHandle_t *pSeqHandle, struct jmsSeqUnit_t *pSeqUnit);
void mnuClearPanelTransitionState(jmsSeqHandle_t *state);
void jmsSeqJumpAndPop(struct jmsSeqUnit_t **pCurrentSeqUnit, struct jmsSeqUnit_t *pSeqUnit);

#endif
