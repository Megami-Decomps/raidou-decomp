#ifndef DDS_ITF_MES_WINDOW_INDEX_H

#define DDS_ITF_MES_WINDOW_INDEX_H
#include "common.h"

/* Signed index into the fixed message-window pool; -1 means no window. */
typedef s32 ItfMesWindowIndex;

#define ITF_MES_WINDOW_INDEX_INVALID ((ItfMesWindowIndex)-1)

#endif /* DDS_ITF_MES_WINDOW_INDEX_H */
