#ifndef DDS_MNU_CAMP_DISPLAY_H

#define DDS_MNU_CAMP_DISPLAY_H
#include "common.h"

typedef struct CampDisplayDefaults {
    s32 x;          /* 0x00 */
    s32 y;          /* 0x04 */
    u8 color[4];    /* 0x08 */
    f32 scaleY;     /* 0x0C */
    f32 scaleX;     /* 0x10 */
    s32 enabled;    /* 0x14 */
    s32 variant;    /* 0x18 */
#ifdef VERSION_DDS2
    s32 preserveAlpha; /* 0x1C: DDS2 RGB-only alpha-fail drawing */
#endif
} CampDisplayDefaults;

#ifdef VERSION_DDS2

typedef char CampDisplayDefaults_size_is_20[
    (sizeof(CampDisplayDefaults) == 0x20) ? 1 : -1];

#else

typedef char CampDisplayDefaults_size_is_1C[
    (sizeof(CampDisplayDefaults) == 0x1C) ? 1 : -1];

#endif

#endif /* DDS_MNU_CAMP_DISPLAY_H */
