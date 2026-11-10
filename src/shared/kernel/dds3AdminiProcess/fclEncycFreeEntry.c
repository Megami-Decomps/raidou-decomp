#include "shared/kernel/dds3AdminiProcess/decls.h"

s32 fclEncycFreeEntry(const char *name)
{
    s32 slotIndex = fclEncycHead;
    s32 characterIndex;

    while (slotIndex >= 0) {
        characterIndex = 0;
        while (fclEncycTable[slotIndex].name[characterIndex] != '\0') {
            if (name[characterIndex] != fclEncycTable[slotIndex].name[characterIndex]) {
                break;
            }
            characterIndex++;
        }
        if (fclEncycTable[slotIndex].name[characterIndex] == '\0' && name[characterIndex] == '\0') {
            return slotIndex;
        }
        slotIndex = fclEncycTable[slotIndex].next;
    }
    return -1;
}
