#include "shared/sdf/sdfDevCons/decls.h"

void sdfDevConsPutInside(sdfDevConsCB_tag *aHndl, s32 aChar, s32 aAttr)
{
    s32 cnt;
    s32 off;

    switch (aChar) {
    case '\t':
        off = (aHndl->locY * aHndl->w + aHndl->locX) * 2;
        cnt = aHndl->locX % aHndl->tabwidth;
        if (cnt == 0) {
            cnt = aHndl->tabwidth;
        }
        do {
            aHndl->bufferPtr[off] = 0;
            aHndl->bufferPtr[off + 1] = aAttr;
            off += 2;
            aHndl->locX++;
            if (aHndl->locX == aHndl->w) {
                sdfDevConsNewLine(aHndl);
                return;
            }
        } while (--cnt);
        return;
    case '\n':
        sdfDevConsNewLine(aHndl);
        return;
    case '\r':
        aHndl->locX = 0;
        return;
    }

    if (aChar == ' ') {
        aChar = 0;
    } else if (aChar < ' ' || aChar > 0x7F) {
        aChar = 0x7F;
    }
    off = (aHndl->locY * aHndl->w + aHndl->locX) * 2;
    aHndl->bufferPtr[off] = aChar;
    aHndl->bufferPtr[off + 1] = aAttr;
    aHndl->locX++;
    if (aHndl->locX == aHndl->w) {
        sdfDevConsNewLine(aHndl);
    }
}
