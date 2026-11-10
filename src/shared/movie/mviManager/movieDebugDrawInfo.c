#include "shared/movie/mviManager/decls.h"

/* One screen of the movie list, with an arrow on the selected row and a
 * marker at each end the list continues past. */
void movieDebugDrawInfo(void)
{
    movieViewerChip_t *chip;
    sdfDrawTag_t *tag;
    s32 selected;
    s32 i;

    if (movieViewerWork.chip == NULL || movieViewerWork.playing != 0) {
        return;
    }
    tag = movieViewerWork.packets;
    sdfDrawTagLink(tag, dds3PrimFrameRect(0x7150, 0x7948, 0xFF0080, 0xF60, 0x3F0, 0x30000000,
                                          0x60404040));
    selected = movieViewerWork.cursor;
    i = movieViewerWork.top;
    chip = movieViewerWork.chip;
    selected -= i;
    for (; i > 0; i--) {
        chip = chip->next;
    }
    for (i = 0; i < 8 && chip != NULL; i++, chip = chip->next) {
        sdfDrawTagLink(tag, sdfDevPrintf(0x7240, 0x79C0 + i * 0x60, 0xFF0080, 0, movieDebugRowFmt,
                                         (i == selected) ? '>' : ' ', chip->path));
    }
    if (movieViewerWork.top != 0) {
        sdfPrimLinkF3(tag, 0x8000A0C0, 0, 0x7900, 0x7978, 0x7840, 0x79A8, 0x79C0, 0x79A8,
                      0xFF0080, 0);
    }
    if (chip != NULL) {
        sdfPrimLinkF3(tag, 0x8000A0C0, 0, 0x7840, 0x7CD8, 0x79C0, 0x7CD8, 0x7900, 0x7D08,
                      0xFF0080, 0);
    }
}
