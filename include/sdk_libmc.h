#ifndef SDK_LIBMC_H
#define SDK_LIBMC_H

#include "common.h"

s32 sceMcChdir(s32 a, s32 b, const char *c, char *d);
s32 sceMcClose(s32 a);
s32 sceMcDelete(s32 port, s32 slot, const char *name);
s32 sceMcFormat(s32 port, s32 slot);
s32 sceMcGetDir(s32 a, s32 b, const char *c, s32 d, s32 e, void *f);
s32 sceMcGetInfo(s32 a, s32 b, s32 *c, s32 *d, s32 *e);
s32 sceMcInit(void);
s32 sceMcMkdir(s32 a, s32 b, const char *c);
s32 sceMcOpen(s32 a, s32 b, const char *c, s32 d);
s32 sceMcRead(s32 a, void *b, s32 c);
s32 sceMcSync(s32 mode, s32 *command, s32 *result);
s32 sceMcWrite(s32 a, const void *b, s32 c);

#endif
