#ifndef FLDCOMMAND_H
#define FLDCOMMAND_H

#include "common.h"
#include "sdf_types.h"

s32 nbCallSetIntReturnValue(void);
s32 nbMAPSETTAKARA(void);
s32 fldCommand_GetHojiID(char *pName);
s32 fldCommand_SET_ALLPANEL(s32 panel);
s32 fldFacilityMistDraw(s32 mode);
s32 gohCheckSequence(void);

#endif
