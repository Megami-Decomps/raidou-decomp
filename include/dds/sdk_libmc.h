#ifndef DDS_SDK_LIBMC_H

#define DDS_SDK_LIBMC_H
/* SDK memory card library (libmc) and the game-named file-stream wrappers linked with it. One canonical prototype per function the game calls;
 * signatures come from retail call-site argument setup, the units' former local
 * declarations and PS2 SDK semantics. */

#include "common.h"

typedef struct SdkFileStream SdkFileStream;

#ifndef SDK_SKIP_sceMcChdir

#endif

#ifndef SDK_SKIP_sceMcDelete

#endif

#ifndef SDK_SKIP_sceMcMkdir

#endif

#ifndef SDK_SKIP_sceMcOpen

#endif

#ifndef SDK_SKIP_sceMcSync

#endif

#ifdef VERSION_DDS2

#ifndef SDK_SKIP_malloc

#endif

#ifndef SDK_SKIP_fclose

void fclose(SdkFileStream *stream);

#endif

#ifndef SDK_SKIP_sdfFreeHeapMemory

#endif

#ifndef SDK_SKIP_fopen

SdkFileStream *fopen(const char *name, const char *mode);

#endif

#ifndef SDK_SKIP_sdfReadFileStream

u32 fread(void *data, u32 size, u32 count, SdkFileStream *stream);

#endif

#ifndef SDK_SKIP_sdfWriteFileStream

u32 fwrite(const void *data, u32 size, u32 count, SdkFileStream *stream);

#endif

#endif

#endif
