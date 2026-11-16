#ifndef SCRSCRIPTPROCESS_H
#define SCRSCRIPTPROCESS_H

#include "common.h"
#include "kernel.h"
#include "scr.h"

scrProcessWork_s *scrRegistScript2(u8 *scrdata);
s32 scrStartProcessProcedure(scrProcessWork_s *pw, s32 procNo);
s32 scrRunScript(scrProcessWork_s *pw);
void scrReleaseScript(scrProcessWork_s *pw);
dds3ProcessID_t *scrRunScriptEx(s32 priority, scrHeader_t *header, scrTypeTable_t *typetable, scrProcTable_t *proctable, scrLabelTable_t *labeltable, scrCode_u *code, u8 *mesdata, u8 *stringdata, s32 startproc);
s32 scrSearchProcedureName(void *scrdata, char *procname);

#endif
