#ifndef DDS_DDS3_ADMIN_CONTROL_API_H

#define DDS_DDS3_ADMIN_CONTROL_API_H
#include "common.h"

/* Administration-task control word; the history-slot flag is separate. */

#define DDS3_ADMIN_CONTROL_REQUEST_PENDING 0x00000001

#define DDS3_ADMIN_CONTROL_EXIT_REQUESTED 0x00000002

#define DDS3_ADMIN_CONTROL_MARK_HISTORY_SLOT_FOR_RESTORE 0x00000004

#define DDS3_ADMIN_CONTROL_KEEP_CURRENT_HISTORY_SLOT 0x00000008

#define DDS3_ADMIN_CONTROL_RESTORE_PREVIOUS_MODE 0x00010000

/* The accessor returns the byte-backed flag field zero-extended as a word. */
typedef u32 Dds3AdminHistoryFlags;

#define DDS3_ADMIN_HISTORY_FLAG_RESTORE_PREVIOUS_MODE 0x01

#endif /* DDS_DDS3_ADMIN_CONTROL_API_H */
