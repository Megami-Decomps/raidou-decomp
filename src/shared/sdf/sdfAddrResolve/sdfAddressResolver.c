#include "shared/sdf/sdfAddrResolve/decls.h"

/* Byte-coded commands advance by word deltas, or relocate a run of following words.
 * Low tag bits select one-, two-, or three-byte deltas; tag 7 encodes a run.
 * tableBytes bounds the encoded byte stream, not the number of relocated words.
 */
void sdfAddressResolver(void *aDataBody, void *aOffset, void *aAddrInfoPtr, s32 aAddrInfoLeng) {
    s32 *words = aDataBody;
    saddr_t relocationBase = EE_ADDR_S(aOffset);
    const u8 *table = aAddrInfoPtr;
    const u8 *commandCursor = table;
    s32 value;
    s32 runIndex;
    while (commandCursor - table < aAddrInfoLeng) {
        value = *commandCursor++;
        if ((value & 1) == 0) {
            value >>= 1;
        } else if ((value & 2) == 0) {
            value = (value | *commandCursor++ << 8) >> 2;
        } else if ((value & 4) == 0) {
            value = (value | commandCursor[0] << 8 | commandCursor[1] << 16) >> 3;
            commandCursor += 2;
        } else {
            value = (value >> 3) + 2;
            for (runIndex = 0; runIndex < value; runIndex++) {
                words++;
                *words += relocationBase;
            }
            continue;
        }
        words += value;
        *words += relocationBase;
    }
}
