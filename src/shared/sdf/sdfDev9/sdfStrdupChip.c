#include "shared/sdf/sdfDev9/decls.h"

char *sdfStrdupChip(const char *text) {
    u32 length;
    char *copy;
    if (text == NULL) {
        return NULL;
    }
    length = strlen(text);
    copy = sdfChipAllocDirect(length + 1);
    memcpy(copy, text, length);
    copy[length] = 0;
    return copy;
}
