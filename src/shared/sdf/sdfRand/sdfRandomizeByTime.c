#include "shared/sdf/sdfRand/decls.h"

/* Seeds the generator at `state` from the current clock (time of day in seconds since an epoch). */
void sdfRandomizeByTime(sdfRandCB_t *aCB) {
    sdfClock_t now;
    s32 secondsSinceEpoch;
    sdfGetCdClock(&now);
    secondsSinceEpoch = now.year * 0x16E + now.day + sdfMathConstants.monthDays[now.month + 7] + 0xFFF4FF83;
    secondsSinceEpoch = secondsSinceEpoch * 0x18 + now.hour;
    secondsSinceEpoch = secondsSinceEpoch * 0x3C + now.min;
    sdfRandomize(aCB, secondsSinceEpoch * 0x3C + now.sec);
}
