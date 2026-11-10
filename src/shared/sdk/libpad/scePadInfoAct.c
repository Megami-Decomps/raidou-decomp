#include "shared/sdk/libpad/decls.h"

int scePadInfoAct(int port, int slot, int actno, int term)
{
    padDma *d;

    if (padPortState[port][slot].open == 0) {
        return 0;
    }
    d = scePadGetDmaStr(port, slot);
    if (d->ok != 1) {
        return 0;
    }
    if (d->ok2 < 2) {
        return 0;
    }
    if (actno >= d->nrAct) {
        return 0;
    }
    if (actno == -1) {
        return d->nrAct;
    }
    switch (term) {
    case 1:
        return d->actInfo[actno][0];
    case 2:
        return d->actInfo[actno][1];
    case 3:
        return d->actInfo[actno][2];
    case 4:
        return d->actInfo[actno][3];
    }
    return 0;
}
