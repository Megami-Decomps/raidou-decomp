#include "shared/event/evtEventPack/decls.h"

dds3ProcessID_t *evtSearchEventPack(u32 eventno) {
    char taskName[32];

    evtMakeEventPackName(eventno, taskName);
    return dds3SearchProcessName((const char *)taskName);
}
