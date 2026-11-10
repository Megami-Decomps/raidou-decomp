#include "shared/interface/itfPanel/decls.h"

/* Set the four color words at byte 0x20, consumed as a color record by the quad path. */
void CreateColorPera1(itfPanelPlainData_t *pwork, s32 red, s32 green, s32 blue, s32 alpha) {
    u32 *color = pwork->colors[0].components;

    color[0] = red;
    color[1] = green;
    color[2] = blue;
    color[3] = alpha;
}
