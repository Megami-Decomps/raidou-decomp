#include "shared/interface/frFont/decls.h"

/* Count characters until a zero lead byte. Any negative signed lead consumes
 * two bytes without checking its trailer; input must be a valid packed stream. */
s32 frStrLen(s8 *pStr) {
    s32 characterCount = 0;

    while (*pStr != 0) {
        if (*pStr >= 0) {
            pStr++;
        } else {
            pStr += 2;
        }
        characterCount++;
    }
    return characterCount;
}
