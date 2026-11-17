#ifndef ITFTESTMESMANAGER_H
#define ITFTESTMESMANAGER_H

#include "common.h"
#include "kernel.h"

void itfDebugMesMngInitialize(void);
void TestMsgReloadTexture(void);
s32 TestMsgMngC_Process(dds3ProcessID_t *id);
s32 TestMsgMngD_Process(dds3ProcessID_t *id);
void itfDebugPrintMark(void);

#endif
