#ifndef SCRSCRIPTPROCESS_H
#define SCRSCRIPTPROCESS_H

#include "common.h"
#include "kernel.h"
#include "scr.h"

struct scrHandle_t;

struct scrHandle_t *scrRegistScript2(u8 *scrdata);
void scrReleaseScript(scrProcessWork_s *pw);
dds3ProcessID_t *scrRunScriptEx(s32 priority, scrHeader_t *header, scrTypeTable_t *typetable, scrProcTable_t *proctable, scrLabelTable_t *labeltable, scrCode_u *code, u8 *mesdata, u8 *stringdata, s32 startproc);

#endif
