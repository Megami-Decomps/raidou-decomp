#include "shared/sdf/sdfIpu/decls.h"

/* Query for a complete header before reading it. Texture dimensions are rounded
 * only when larger than one GS page; smaller dimensions remain unchanged.
 */
void sdfIpuAllocGsRect(sdfIpuDecodeCb_t *node) {
    sdfStreamFrameHeader_t header;
    s32 readStatus;
    s32 interruptsEnabled;
    if (node->headerReady != 0) {
        return;
    }
    if (node->read(node, node->strmCallbackParm, SDF_STREAM_READ_QUERY, &readStatus, 0) < sizeof(header)) {
        return;
    }
    node->headerReady = 1;
    node->read(node, node->strmCallbackParm, SDF_STREAM_READ_COPY, &header, sizeof(header));
    node->width = header.width;
    node->height = header.height;
    node->frameCount = header.cycleLength;
    if (node->resourceWord == 0) {
        s32 textureWidth = node->width;
        s32 textureHeight = node->height;
        s32 pixelFormat;
        sdfGsMemSlice_t *texture;
        if (textureWidth > 64) {
            textureWidth = (textureWidth + 63) & ~63;
        }
        if (node->rgbMode == SDF_STREAM_OUTPUT_RGBA32) {
            pixelFormat = 0;
            if (textureHeight > 32) {
                textureHeight = (textureHeight + 31) & ~31;
            }
        } else {
            pixelFormat = 2;
            if (textureHeight > 64) {
                textureHeight = (textureHeight + 63) & ~63;
            }
        }
        texture = sdfGsMemAllocRect(textureWidth, textureHeight, pixelFormat, SDF_TEX_RESOURCE_TEXTURE, 0);
        node->textureHead = texture;
        node->resourceWord = texture->addr;
    }
    sdfIpuAllocBuff(node);
    interruptsEnabled = DIntr();
    sdfIpuAttachQueue(node, 0);
    if (interruptsEnabled != 0) {
        EIntr();
    }
}
