#include "shared/sdf/sdfRand/decls.h"

s32 sdfGetCdClock(sdfClock_t *now) {
    sdfCdClock_t clock;
    s32 result = 1;
    s32 year;
    if (sdfCdClock == 0) {
        sceCdReadClock(&clock);
        result = -1;
        if ((clock.status & 0x103) == 0) {
            sdfCdClockCache.sec = sdfBcdToDecimal(clock.second);
            sdfCdClockCache.min = sdfBcdToDecimal(clock.minute);
            sdfCdClockCache.hour = sdfBcdToDecimal(clock.hour);
            sdfCdClockCache.day = sdfBcdToDecimal(clock.day);
            sdfCdClockCache.month = sdfBcdToDecimal(clock.month);
            /* Retail reuses the month byte as the two-digit year input. */
            year = sdfBcdToDecimal(clock.month);
            result = 0;
            sdfCdClockCache.year = year + (year < 70 ? 2000 : 1900);
        }
        sdfCdClock = 20;
    }
    *now = sdfCdClockCache;
    return result;
}
