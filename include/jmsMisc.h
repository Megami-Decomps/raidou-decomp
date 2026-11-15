#ifndef JMSMISC_H
#define JMSMISC_H

#include "common.h"
#include "code_r1_001CC1D0.h"

struct jmsSeqHandle_t;
struct jmsSeqUnit_t;

u32 jmsChkCommonInput(u32 ControlBit);
s32 jmsChkSeqChange(struct jmsSeqUnit_t *pSeqUnit);
s32 jmsSeqChange(MenuPopupState *pSeqHandle, s32 *pSeqUnit, s32 Mode, void *ID);
s32 jmsSeqChkLast(struct jmsSeqHandle_t *pSeqHandle, struct jmsSeqUnit_t *pSeqUnit);
void jmsSeqJumpAndPop(struct jmsSeqUnit_t **pCurrentSeqUnit, struct jmsSeqUnit_t *pSeqUnit);

#endif
