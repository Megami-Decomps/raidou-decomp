#include "shared/sdk/libpad/decls.h"

int scePadInfoMode(int port, int slot, int term, int offs)
{
    padDma *d;

    if (padPortState[port][slot].open == 0) {
        return 0;
    }
    d = scePadGetDmaStr(port, slot);
    if (d->ok != 1) {
        return 0;
    }
    if (d->reqState == 2) {
        return 0;
    }
    switch (term) {
    case 1:
        if (d->modeCurId == 0xF3) {
            return 0;
        }
        return d->modeCurId >> 4;
    case 2:
        if (d->ok2 == 1) {
            return 0;
        }
        return d->modeTable[d->modeCurOffs];
    case 3:
        if (d->ok2 == 1) {
            return 0;
        }
        return d->modeCurOffs;
    case 4:
        if (d->ok2 == 1) {
            return 0;
        }
        if (offs == -1) {
            return d->nrMode;
        }
        if (offs >= d->nrMode) {
            return 0;
        }
        return d->modeTable[offs];
    }
    return 0;
}
