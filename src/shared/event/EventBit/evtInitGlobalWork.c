#include "shared/event/EventBit/decls.h"

void evtInitGlobalWork(void) {
    evtBitAllOff();
    evtSetAgeOfMoon(0);
    evtDisableMoonDraw();
    evtDisableMoonCalc();
}
