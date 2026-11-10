#include "shared/field/fldCommand/decls.h"

s32 fldCommand_GetHojiID(char *pName) {
    s32 index;
    if (strstr(pName, fldHojiNameKey) == 0) return 0;
    for (index = 0; index < 32; index++) {
        if (pName[index] == '\0') {
            if (index < 4) return 0;
            return (pName[index - 2] - '0') * 10 + (pName[index - 1] - '0');
        }
    }
    return 0;
}
