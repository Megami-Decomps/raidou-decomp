#include "shared/event/evtCommand/decls.h"

u32 evtCommand_BOKASI_OFF(void) {
    evtSetBokasi(0);
    return 1;
}
