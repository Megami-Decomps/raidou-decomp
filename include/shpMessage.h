#ifndef SHPMESSAGE_H
#define SHPMESSAGE_H

#include "common.h"

struct shpMessageWork_t;
extern struct shpMessageWork_t *wk_shpMessage;

s32 fclCheck(void);
void shpNewsStartScript(void);
void shpOrderStartScript(s32 a_stockID, s32 a_itemID);
void shpStartScript(s32 a_procedureNo);
void fclLeaf(void);

#endif
