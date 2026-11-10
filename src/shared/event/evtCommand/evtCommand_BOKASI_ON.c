#include "shared/event/evtCommand/decls.h"

u32 evtCommand_BOKASI_ON(void) {
    evtSetBokasi(1);
    return 1;
}
