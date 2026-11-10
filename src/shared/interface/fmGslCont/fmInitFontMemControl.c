#include "shared/interface/fmGslCont/decls.h"

/* Build the glyph atlas once: COLUMNS x ROWS cells in a circular node list, each with its
 * UV rectangle in 1/16-texel units and an optional RECORDBYTES-sized per-cell record. */
s32 fmInitFontMemControl(s32 unused, s32 columns, s32 rows, s32 cellWidth, s32 cellHeight,
                               s32 recordBytes) {
    u8 *records = NULL;
    FontMem_t *node;
    s32 count;
    s32 i;
    s32 row;
    s32 column;

    if (gFGC.root != NULL && fmFontMemInitialized != 0) {
        return 0;
    }
    count = columns * rows;
    gFGC.unit_width = cellWidth;
    gFGC.unit_height = cellHeight;
    gFGC.unit_nums = count;
    gFGC.unit_num_max = count;
    gFGC.unit_size = cellWidth * cellHeight / 2;
    gFGC.mh = sdfMemAllocDirect(count * sizeof(FontMem_t));
    gFGC.root = (FontMem_t *)sdfMemLock(gFGC.mh);
    if (recordBytes != 0) {
        gFGC.ud_mh = sdfMemAllocDirect(recordBytes * gFGC.unit_nums);
        records = (u8 *)sdfMemLock(gFGC.ud_mh);
    }
    for (i = 0; i < gFGC.unit_nums - 1; i++) {
        gFGC.root[i].next = &gFGC.root[i + 1];
    }
    gFGC.root[i].next = gFGC.root;
    for (i = gFGC.unit_nums - 1; i > 0; i--) {
        gFGC.root[i].prev = &gFGC.root[i - 1];
    }
    gFGC.root[0].prev = &gFGC.root[gFGC.unit_nums - 1];
    i = 0;
    for (row = 0; row < rows; row++) {
        for (column = 0; column < columns; column++) {
            node = &gFGC.root[i];
            if (node == NULL) {
                fmDestroyFontMemControl();
                return 0;
            }
            node->id = i;
            node->uv.u0 = (column * cellWidth) << 4;
            node->uv.u1 = ((column * cellWidth) << 4) + (cellWidth << 4);
            if (records != NULL) {
                node->user_data = (GsFont_t *)(records + i * recordBytes);
            } else {
                node->user_data = NULL;
            }
            node->uv.v0 = (row * cellHeight) << 4;
            node->uv.v1 = ((row * cellHeight) << 4) + (cellHeight << 4);
            i++;
        }
    }
    fmFontMemInitialized = 1;
    return 1;
}
