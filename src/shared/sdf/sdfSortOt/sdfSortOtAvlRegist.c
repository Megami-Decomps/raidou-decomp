#include "shared/sdf/sdfSortOt/decls.h"

void sdfSortOtAvlRegist(sdfSortOtNode_tag **root_p, sdfDrawTag_t *tag) {
    sdfSortOtNode_tag *path[SDF_KEY_TREE_PATH_LIMIT];
    s32 depth = 0;
    sdfSortOtNode_tag **link = root_p;
    sdfSortOtNode_tag *cur = *root_p;
    sdfSortOtNode_tag *node;
    f32 key = tag->z;

    while (cur != NULL) {
        if (key == cur->tag->z) {
            tag->next = cur->tag;
            cur->tag = tag;
            return;
        }
        path[depth] = cur;
        depth++;
        if (cur->tag->z < key) {
            link = &cur->right;
        } else {
            link = &cur->left;
        }
        cur = *link;
    }
    tag->next = NULL;
    node = EE_ADDR_PTR(sdfGrAllocPacketBuff(0x10));
    node->right = NULL;
    node->left = NULL;
    node->tag = tag;
    *link = node;
    node->balance = 0;
    if (depth != 0) {
        sdfSortOtAvlCheckBalance(path, depth, node, root_p);
    }
}
