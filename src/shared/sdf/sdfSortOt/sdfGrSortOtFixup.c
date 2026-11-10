#include "shared/sdf/sdfSortOt/decls.h"

void sdfGrSortOtFixup(void *tag, sdfDrawTag_t **head_p, sdfDrawTag_t **tail_p) {
    sdfDrawTag_t *current;
    sdfDrawTag_t *next;

    if (tag != NULL) {
        if (*tail_p == NULL) {
            *head_p = tag;
        } else {
            sdfGrJoinDrawTag(*tail_p, tag);
        }
        current = tag;
        while ((next = current->next) != NULL) {
            sdfGrJoinDrawTag(current, next);
            current = next;
        }
        *tail_p = current;
    }
}
