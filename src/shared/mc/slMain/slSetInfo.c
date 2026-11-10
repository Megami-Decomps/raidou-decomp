#include "shared/mc/slMain/decls.h"

void slSetInfo(u32 id) {
    s32 previous;

    previous = gslInfoCode;
    gslInfoCode = id;
    if (previous == 0) {
        gSlWindowCnt = 0;
    }
}
