#include "common.h"
#include "newdata.h"
#include "scrCommand.h"

s32 scrRefDDS3GBWK(void) {
    DDS3_GBWK->integers[210] = 0;
    return 1;
}

s32 scrRefDDS3GBWK2(void) {
    DDS3_GBWK->integers[210] = 1;
    return 1;
}

void scrSetMovieCancel(s32 Mode) {
    if (Mode == 0) {
        scrRefDDS3GBWK2();
    } else {
        scrRefDDS3GBWK();
    }
}

s32 scrRefDDS3GBWK3(void) {
    return DDS3_GBWK->integers[210] == 0;
}
