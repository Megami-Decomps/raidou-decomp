#ifndef CMPSTATUSITMSKLSET_H
#define CMPSTATUSITMSKLSET_H

#include "common.h"

struct tagCMP_STATUS_WORK;

void CmpStatusDraw(s32 a_alphaRate); /* passed down as the sprite alpha */
void cmpLvUpSeqFadeInStepUpdate(struct tagCMP_STATUS_WORK *wk);

#endif
