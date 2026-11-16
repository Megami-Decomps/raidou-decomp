#ifndef CODE_R1_002C7ED0_H
#define CODE_R1_002C7ED0_H

#include "common.h"

/* Resource jobs retain a destination slot until their request completes. */
typedef struct effListNode_t {
    u32 state;
    struct effListNode_t *next;
    const char *value;
    const char *length; /* the resource name, or the package path in modes 1 and 2 */
    u32 kind;
    void **reference;
} effListNode_t;

typedef struct effList_t {
    u32 mode;
    s32 count;
    effListNode_t *first;
    effListNode_t *last;
    struct fileRequest *request;
} effList_t;

typedef char effListNode_size_must_be_0x18[(sizeof(effListNode_t) == 0x18) ? 1 : -1];

typedef char effList_size_must_be_0x14[(sizeof(effList_t) == 0x14) ? 1 : -1];

s32 effPollResourceList(struct effList_t *list);
s32 effAppendListEntry(struct effList_t *list, const char *value, const char *length, u32 kind, void **reference);

#endif
