#ifndef DDS3SEQUENCELIST_00111FF8_H
#define DDS3SEQUENCELIST_00111FF8_H

#include "common.h"
#include "scr.h"

struct scrCode_t;
struct scrHandle_t;

struct scrHandle_t *AddScript(scrHeader_t *header, scrTypeTable_t *typetable, scrProcTable_t *proctable, scrLabelTable_t *labeltable, struct scrCode_t *code, u8 *mesdata, u8 *stringdata, s32 startproc);

void scrLinkHandle(scrProcessWork_s *work);
void scrUnlinkHandle(scrProcessWork_s *work);

#endif
