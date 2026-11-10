#include "shared/effect/effMagatuhi/decls.h"

dds3MagatuhiBill_tag *effMagaCylinderAllocTex(s32 count, s32 historyCount, f32 historyDistanceScale, s32 param, f32 historyAngleStep) {
    s32 countFrames;
    s32 frameTerm;
    s32 countTerm;
    s32 allocationSize;
    struct sdfMemHandle_t *allocation;
    s32 positions;
    s32 colorTable;
    s32 historyScalars;
    s32 values;
    s32 writeIndices;
    s32 validCounts;
    s32 angleValues;
    dds3MagatuhiBill_tag *resource;
    s16 *defaults;
    u32 i;

    countFrames = count * historyCount;
    frameTerm = countFrames + historyCount;
    countTerm = (countFrames << 2) + count;
    allocationSize = (((count << 3) + ((countTerm + frameTerm) << 1) + (count << 1)) << 1) + sizeof(dds3MagatuhiBill_tag);
    allocation = sdfMemAllocDirect(allocationSize);
    positions = sdfMemLock(allocation);
    colorTable = positions + (countFrames << 4);
    historyScalars = colorTable + (historyCount << 2);
    values = historyScalars + (countFrames << 2);
    writeIndices = values + (count << 2);
    validCounts = writeIndices + (count << 1);
    angleValues = validCounts + (count << 1);
    resource = EE_ADDR_AS(dds3MagatuhiBill_tag *, (angleValues + (count << 4)));

    resource->count = count;
    resource->historyCount = historyCount;
    resource->historyAngleStep = historyAngleStep;
    resource->historyDistanceScale = historyDistanceScale;
    resource->drawBucket = param;
    resource->positions = EE_ADDR_AS(effMagatuhiRow_t *, positions);
    resource->colorTable = EE_ADDR_AS(u32 *, colorTable);
    resource->historyScalars = EE_ADDR_AS(f32 *, historyScalars);
    resource->slotColors = EE_ADDR_AS(u32 *, values);
    resource->writeIndices = EE_ADDR_AS(u16 *, writeIndices);
    resource->validCounts = EE_ADDR_AS(u16 *, validCounts);
    resource->angleRows = EE_ADDR_AS(effMagatuhiRow_t *, angleValues);
    resource->allocationHandle = allocation;
    resource->texture = dds3BillboardGetGeneralTex(0);

    defaults = &effMagaBillboardTexCoords[0][0];
    i = 0;
    do {
        i++;
        defaults[0] = 0;
        defaults[1] = 0;
        defaults[2] = 0x400;
        defaults[3] = 0;
        defaults[4] = 0x400;
        defaults[5] = 0x400;
        defaults[6] = 0;
        defaults[7] = 0x400;
        defaults += 8;
    } while (i < 0xF);

    if (count != 0) {
        i = 0;
        do {
            effMagaCylinderSetPartPtrs(resource, i);
            i++;
        } while (i < count);
    }
    return resource;
}
