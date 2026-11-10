#ifndef DDS_SDK_LIBC_H

#define DDS_SDK_LIBC_H
/* SDK C library (libc). One canonical prototype per function the game calls;
 * signatures come from retail call-site argument setup, the units' former local
 * declarations and PS2 SDK semantics. */

#include "common.h"

#ifndef SDK_SKIP_memcmp

#endif

#ifndef SDK_SKIP_memcpy

#endif

#ifndef SDK_SKIP_memmove

#endif

#ifndef SDK_SKIP_memset

#endif

#ifndef SDK_SKIP_sdfVFormatString

#endif

#ifndef SDK_SKIP_sprintf

#endif

#ifndef SDK_SKIP_sscanf

s32 sscanf(const char *text, const char *format, ...);

#endif

#ifndef SDK_SKIP_strcasecmp

#endif

#ifndef SDK_SKIP_strcat

#endif

#ifndef SDK_SKIP_strchr

#endif

#ifndef SDK_SKIP_strcmp

#endif

#ifndef SDK_SKIP_strcpy

#endif

#ifndef SDK_SKIP_strlen

#endif

#ifndef SDK_SKIP_strncmp

#endif

#ifndef SDK_SKIP_strrchr

#endif

#ifndef SDK_SKIP_strstr

#endif

#ifdef VERSION_DDS2

#ifndef SDK_SKIP_puts

#endif

#ifndef SDK_SKIP_qsort

#endif

#ifndef SDK_SKIP_rand

s32 rand(void);

#endif

#ifndef SDK_SKIP_strncpy

#endif

#endif

#endif
