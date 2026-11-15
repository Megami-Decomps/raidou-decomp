#ifndef CMPFILE_H
#define CMPFILE_H

#include "common.h"
#include "enums.h"

struct sprHandle_t;

struct sprHandle_t *cmpGetSpriteHnd(CMP_SPR a_no);
s32 cmpLoadSpriteHnd(CMP_SPR a_no);

#endif
