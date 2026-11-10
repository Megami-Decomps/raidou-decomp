#include "shared/sdk/libkernl/decls.h"

char *SetArg(const char *path, s32 count, const char *const *arguments)
{
    char **table = ((execArgumentBlock_t *)execArgumentBlock)->arguments;
    char *cursor = (char *)&table[16];
    char *start = cursor;
    u32 length;

    PatchIsNeeded(setArgEntryPairs[0].index, setArgEntryPairs[0].entry);
    if (count >= 16) {
        count = 15;
    }
    gcc2_compiled__16(table, &cursor, 4);
    table++;
    length = strlen(path) + 1;
    gcc2_compiled__16(cursor, path, length);
    cursor += length;
    if (count > 0) {
        const char *const *argument = arguments;

        do {
            gcc2_compiled__16(table++, &cursor, 4);
            count--;
            length = strlen(*argument) + 1;
            gcc2_compiled__16(cursor, *argument++, length);
            cursor += length;
        } while (count != 0);
    }
    return start;
}
