#include "shared/sdf/sdfDevCons/decls.h"

void sdfDevConsDraw(sdfDevConsCB_tag *console, sdfDevConsDrawCB_t *buffers) {
    u8 *cells;
    vu64 *writeCursor;
    u8 *packetStart;
    s32 row;
    s32 column;
    s32 y;
    s32 cellStep;
    u64 top;
    s32 character;
    s32 attribute;
    u64 glyph;
    u64 color;

    if ((u8)(console->mode & 1)) {
        return;
    }
    buffers->stepY = (console->mode & 4) ? 0xC0 : 0x60;
    if (console->mode & 2) {
        sdfDevConsDrawBG(console, buffers);
    }
    cellStep = ((u32)buffers->stepY << 16) | 0xC0;
    row = 0;
    writeCursor = (vu64 *)buffers->cur;
    packetStart = buffers->start;
    cells = console->bufferPtr;
    y = console->y;
    do {
        top = (u32)console->x | ((u32)y << 16);
        column = 0;
        do {
            character = cells[0];
            attribute = cells[1];
            cells += 2;
            if (character >= 0x20) {
                if (buffers->buffSize < (u8 *)writeCursor - packetStart + 0x50) {
                    buffers->cur = (u8 *)writeCursor;
                    sdfDevConsDrawFlush(buffers);
                    writeCursor = (vu64 *)buffers->cur;
                    packetStart = buffers->start;
                }
                glyph = sdfDevConsFontUvTable[character - 0x20];
                color = sdfDevConsColorTable[attribute];
                writeCursor[0] = color;
                writeCursor[1] = glyph;
                writeCursor[2] = top;
                writeCursor[3] = glyph + 0xC000C0;
                writeCursor[4] = top + cellStep;
                writeCursor += 5;
            }
            column++;
            top += 0xC0;
        } while (column != console->w);
        row++;
        y += buffers->stepY;
    } while (row != console->h);
    buffers->cur = (u8 *)writeCursor;
}
