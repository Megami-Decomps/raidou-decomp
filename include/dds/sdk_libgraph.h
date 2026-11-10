#ifndef DDS_SDK_LIBGRAPH_H

#define DDS_SDK_LIBGRAPH_H
/* SDK graphics synthesizer library (libgraph). One canonical prototype per function the game calls;
 * signatures come from retail call-site argument setup, the units' former local
 * declarations and PS2 SDK semantics. */

#include "common.h"

#if !defined(SMT3_DEFINES_sceGsStoreImage)
typedef struct sceGsStoreImage sceGsStoreImage;
#endif

#ifndef SDK_SKIP_sceGsExecLoadImage

#endif

#ifndef SDK_SKIP_sceGsExecStoreImage

#endif

#ifndef SDK_SKIP_sceGsPutDispEnv

#endif

#ifndef SDK_SKIP_sceGsResetGraph

#endif

#ifndef SDK_SKIP_sceGsResetPath

#endif

#ifndef SDK_SKIP_sceGsSetDefDispEnv

#endif

#ifndef SDK_SKIP_sceGsSetDefLoadImage

#endif

#ifndef SDK_SKIP_sceGsSetDefStoreImage

#endif

#ifndef SDK_SKIP_sceGsSyncPath

#endif

#endif
