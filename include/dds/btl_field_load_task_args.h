#ifndef DDS_BTL_FIELD_LOAD_TASK_ARGS_H

#define DDS_BTL_FIELD_LOAD_TASK_ARGS_H
#include "common.h"

#include "sdf_texture_offset_list.h"

struct FilePacRequest;

/* Complete 0x28-byte payload allocated by the paired field-load task creators. */
typedef struct BtlFieldLoadArgs {
    s32 stage;                         /* 0x00: area used to form the field path */
    s32 variant;                       /* 0x04: field variant used in the path */
    struct FilePacRequest *request; /* 0x08: pending PAC archive request */
    u8 pad0C[0xC];                     /* 0x0C: opaque state retained by the task */
    u8 *fieldF1;                       /* 0x18: loaded F1 resource bytes */
    u8 *fieldF2;                       /* 0x1C: loaded F2 resource bytes */
    const SdfTextureOffsetListHeader *fieldTB; /* 0x20: loaded texture-offset resource */
    s32 frame;                         /* 0x24: callback phase/frame */
} BtlFieldLoadArgs;

typedef char BtlFieldLoadArgs_size[(sizeof(BtlFieldLoadArgs) == 0x28) ? 1 : -1];

#endif /* DDS_BTL_FIELD_LOAD_TASK_ARGS_H */
