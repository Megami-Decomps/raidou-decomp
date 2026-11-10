#include "shared/sdf/sdfVu1/decls.h"

void corePktSetConstColor(sdfCorePacketCb_t *cb, u32 color) {
    f32 colorScale = 1.0f / 128.0f;
    sdfCorePacketPointBuff_tag *node;
    s32 remaining;
    u128 scaledColor;

    EE_MMI_RGBA_MULTIPLY_VECTOR(scaledColor, cb->color, color, colorScale);

    remaining = cb->pointCount;
    node = EE_ADDR_AS(sdfCorePacketPointBuff_tag *, cb->pointBuff);
    if (remaining != 0) {
        do {
            remaining--;
            *(u128 *)node->color = scaledColor;
            node++;
        } while (remaining != 0);
    }

    node = cb->extPointTop;
    while (node != NULL) {
        sdfCorePacketPointBuff_tag *source = (sdfCorePacketPointBuff_tag *)node->sourceA;
        SDF_VU0_COPYVECTOR_MEMOPS(node->color, source->color);
        node = node->next;
    }
}
