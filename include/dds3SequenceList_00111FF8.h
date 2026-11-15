#ifndef DDS3SEQUENCELIST_00111FF8_H
#define DDS3SEQUENCELIST_00111FF8_H

#include "common.h"
#include "scr.h"

scrProcessWork_s *AddScript(scrHeader_t *header, scrTypeTable_t *typetable, scrProcTable_t *proctable, scrLabelTable_t *labeltable, scrCode_u *code, u8 *mesdata, char *strdata, s32 procno);

void scrLinkHandle(scrProcessWork_s *work);
void scrUnlinkHandle(scrProcessWork_s *work);

#endif
