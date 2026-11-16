#ifndef CFG_H
#define CFG_H

#include "common.h"

extern s32 cfgData;
extern u32 cfgWork;

void cfgCopyFlagGblToBak(void);
void cfgCopyFlagBakToGbl(void);
s32 dds3ToggleConfigFlagSub(s32 type, u32 *flag);
s32 dds3RefDDS3GBWK(s32 type);
s32 dds3RefDDS3GBWK2(s32 type, s32 on);

#endif /* CFG_H */
