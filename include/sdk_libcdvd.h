#ifndef SDK_LIBCDVD_H
#define SDK_LIBCDVD_H

#include "common.h"

typedef struct sceCdlFILE sceCdlFILE;

s32 sceCdSearchFile(sceCdlFILE *file, const char *path);
s32 sceCdStRead(u32 sectors, u32 *buffer, u32 mode, u32 *error);

s32 sceCdStStop(void);

s32 _sceCd_scmd_prechk(s32 command);

s32 sceCdStatus(void);

#endif
