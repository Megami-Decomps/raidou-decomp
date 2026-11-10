#include "shared/sdk/libpad/decls.h"

int scePadInfoComb(int port, int slot, int listno, int offs)
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
    if (listno == -1) {
        return d->nrComb;
    }
    if (listno >= d->nrComb) {
        return 0;
    }
    switch (offs) {
    case -1:
        return d->combInfo[listno][0];
    case 0:
        return d->combInfo[listno][1];
    case 1:
        return d->combInfo[listno][2];
    case 2:
        return d->combInfo[listno][3];
    }
    return 0;
}
