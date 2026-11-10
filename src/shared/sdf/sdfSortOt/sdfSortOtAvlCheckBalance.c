#include "shared/sdf/sdfSortOt/decls.h"

void sdfSortOtAvlCheckBalance(sdfSortOtNode_tag **stack, s32 stack_depth, sdfSortOtNode_tag *node,
                   sdfSortOtNode_tag **root_p) {
    sdfSortOtNode_tag *parent;
    sdfSortOtNode_tag *replacement = NULL;
    s32 balance;

    if (stack_depth > 0) {
        do {
            parent = stack[--stack_depth];
            if (node == parent->right) {
                balance = ++parent->balance;
            } else {
                balance = --parent->balance;
            }
            if (balance == 0) {
                return;
            }
            if (balance > 1) {
                replacement = sdfSortOtAvlRotateLR(node, parent);
                break;
            }
            if (balance < -1) {
                replacement = sdfSortOtAvlRotateRL(node, parent);
                break;
            }
            node = parent;
        } while (stack_depth > 0);
    }
    if (stack_depth > 0) {
        sdfSortOtNode_tag *ancestor = stack[--stack_depth];
        if (ancestor->right == parent) {
            ancestor->right = replacement;
        } else {
            ancestor->left = replacement;
        }
    } else if (replacement != NULL) {
        *root_p = replacement;
    }
}
