#ifndef DDS_SDF_MODEL_H

#define DDS_SDF_MODEL_H
#include "../model.h"
#include "sdf_draw.h"

/* Native model constructor reference: AMB roots and the FLD model wrapper's
 * four-word subrecord. The item list has a variable-length body of SdfItem
 * records beginning at its firstItem array. */
typedef struct SdfItemList SdfItemList;

/* Values understood by sdfModelNodeSet. */

#define SDF_ITEM_COMMAND_SETUP_INLINE_LISTS 0

#define SDF_ITEM_COMMAND_SETUP_NULL_TERMINATED_LIST_TABLE 1

/* Per-item view after packed-resource or scene relocation, applied by
 * sdfModelNodeBaseSet. Command-list fields hold relocated pointers. */
typedef struct SdfItem {
    u16 commandSetupMode; /* 0x00: selects the optional command-list form */
    u16 reserved02;       /* 0x02 */
    u32 unk04;            /* 0x04 */
    s32 nodeId;       /* 0x08 */
    s32 parentNodeIndex; /* 0x0C: negative means this node has no parent */
    f32 rotationX;    /* 0x10: Euler angles supplied to the quaternion builder */
    f32 rotationY;    /* 0x14 */
    f32 rotationZ;    /* 0x18 */
    u8 pad1C[4];
    u128 translation; /* 0x20 */
    u128 scale;       /* 0x30 */
    const f32 *boundsCorners; /* 0x40: optional two xyz corners used by clipping */
    union {
        u32 *inlineCommandLists[3]; /* SDF_ITEM_COMMAND_SETUP_INLINE_LISTS */
        struct {
            u32 **commandLists; /* SDF_ITEM_COMMAND_SETUP_NULL_TERMINATED_LIST_TABLE */
            u32 unk48;
            u32 unk4C;
        } commandList;
    } commandData; /* 0x44: other modes leave the command tail unused */
} SdfItem;

typedef char SdfItem_size_must_be_0x50[(sizeof(SdfItem) == 0x50) ? 1 : -1];

/* Variable-length serialized list: a 0x10-byte header, followed by count
 * complete 0x50-byte items. The array contributes no bytes to the header. */
struct SdfItemList {
    s32 count;
    u8 pad_0x04[0x0C];
    SdfItem firstItem[0];
};

typedef char SdfItemList_header_size_must_be_0x10[
    (sizeof(SdfItemList) == 0x10) ? 1 : -1];

#endif /* DDS_SDF_MODEL_H */
