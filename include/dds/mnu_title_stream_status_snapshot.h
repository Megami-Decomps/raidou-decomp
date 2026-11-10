#ifndef DDS_MNU_TITLE_STREAM_STATUS_SNAPSHOT_H

#define DDS_MNU_TITLE_STREAM_STATUS_SNAPSHOT_H
#include "common.h"

typedef struct TitleAudioStreamStatusSnapshot {
    u32 frameCount;
    u32 frameIndex;
    u32 repeatFrame;
} TitleAudioStreamStatusSnapshot;

typedef char TitleAudioStreamStatusSnapshot_size[
    (sizeof(TitleAudioStreamStatusSnapshot) == 0x0C) ? 1 : -1];

#endif
