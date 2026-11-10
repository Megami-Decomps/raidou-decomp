#include "shared/sdf/sdfDevCons/decls.h"

void sdfDevConsDrawAll(void) {
    sdfDevConsStackPackets_t storage;
    sdfDevConsDrawCB_t packetBuffers;
    sdfDevConsCB_tag *console;

    if (devConsTop != NULL) {
        sdfDevConsDrawInit(&packetBuffers, storage.data, 0x1000);
        for (console = devConsTop; console != NULL; console = console->next) {
            sdfDevConsDraw(console, &packetBuffers);
        }
        sdfDevConsDrawFlush(&packetBuffers);
    }
}
