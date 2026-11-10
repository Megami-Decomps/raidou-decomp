#include "shared/effect/effMagatuhi/decls.h"

void effSwordTrackDrawTex(dds3MagatuhiBill_tag *work) {
    sdfVector4_t positions[15];
    u32 colors[15];
    f32 offsets[15][8];
    f32 matrix[16];
    sdfVector4_t point;
    sdfDrawTag_t *list;
    u32 packet;
    u32 *vertexPacket;
    f32 (*historyPositions)[4];
    f32 *widthFactors;
    u32 *palette;
    u32 *slotColors;
    u16 *writeIndices;
    u16 *validCounts;
    u32 historyOffset;
    u32 particleIndex;
    u32 particleCount;
    u32 slotColor;
    u32 validCount;
    u32 historyCount;
    s32 pendingCount;
    f32 baseSize;
    u16 index, historyIndex;
    f32 size, widthScale;
    u32 packed;

    list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(list);
    packet = sdfGrAllocPacketBuff(0x20);
    sdf3DMakeDirectRefPacket(EE_ADDR_AS(dmaPacketHeader_t *, packet),
        EE_ADDR(sdfTexGetPacket(work->texture)),
        sdfTexGetPacketLength((sdfTexHandle_t *)work->texture));
    sdfDrawTagLinkRef(list, packet);
    SDF_VU0_STOREIDENTITY(matrix);
    historyOffset = 0;
    packet = sdfGrAllocPacketBuff(0x38);
    sdfBillBoardMakeTMPacket(EE_ADDR_AS(u8 *, packet), matrix);
    sdfDrawTagLink(list, packet);
    pendingCount = 0;
    particleCount = work->count;
    historyCount = work->historyCount;
    baseSize = work->historyDistanceScale;
    palette = work->colorTable;
    slotColors = work->slotColors;
    writeIndices = work->writeIndices;
    validCounts = work->validCounts;
    for (particleIndex = 0; particleIndex < particleCount; particleIndex++) {
        slotColor = *slotColors++;
        historyPositions = work->positions + historyOffset;
        widthFactors = work->historyScalars + historyOffset;
        validCount = *validCounts++;
        index = *writeIndices++;
        historyOffset += historyCount;
        if (slotColor & 0xFF000000) {
            SDF_VU0_COPYVECTOR(&point, historyPositions[index]);
            historyIndex = 0;
            if (validCount != 0) {
                widthScale = baseSize * 4.0f;
                do {
                    f32 factor = widthFactors[index];
                    widthFactors[index] = factor * 0.75f + 0.25f;
                    size = widthScale * factor;
                    if (index == 0) {
                        index = historyCount - 1;
                    } else {
                        index--;
                    }
                    SDF_VU0_LOADVECTOR(&point);
                    SDF_VU0_STOREVECTOR(&positions[pendingCount]);
                    SDF_VU0_LOADVECTOR_B(historyPositions[index]);
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(&point);
                    UnpackRGBAWordFloat(slotColor, 0.0078125f);
                    SDF_VU0_COPYVECTOR_A_TO_B();
                    UnpackRGBAWordFloat(palette[historyIndex], 0.0078125f);
                    SDF_VU0_MULVECTOR();
                    packed = PackMagatuhiTrailColor();
                    colors[pendingCount] = packed;
                    offsets[pendingCount][0] = -size;
                    offsets[pendingCount][1] = -size;
                    offsets[pendingCount][2] = size;
                    offsets[pendingCount][3] = -size;
                    offsets[pendingCount][4] = size;
                    offsets[pendingCount][5] = size;
                    offsets[pendingCount][6] = -size;
                    offsets[pendingCount][7] = size;
                    pendingCount++;
                    if (pendingCount == 15) {
                        vertexPacket = sdfBillBoardMakeN((u128 *)positions,
                            colors, effMagaBillboardTexCoords, offsets, 15, 0);
                        sdfDrawTagLink(list, EE_ADDR(vertexPacket));
                        pendingCount = 0;
                    }
                    historyIndex++;
                } while (historyIndex < validCount);
            }
        }
    }
    if (pendingCount != 0) {
        vertexPacket = sdfBillBoardMakeN((u128 *)positions,
            colors, effMagaBillboardTexCoords, offsets, pendingCount, 0);
        sdfDrawTagLink(list, EE_ADDR(vertexPacket));
    }
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))effMagaGrOtBuckets[work->drawBucket]->regFunc)(effMagaGrOtBuckets[work->drawBucket], list);
}
