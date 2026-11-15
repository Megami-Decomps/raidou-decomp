#ifndef SBTLSM_H
#define SBTLSM_H

#include "common.h"

struct SBTLSM_LISTWORK;

s32 sbtlDeleteSMList(struct SBTLSM_LISTWORK *lwk);
s32 sbtlReleaseSM_List(struct SBTLSM_LISTWORK *lwk, u8 flg);

#endif
