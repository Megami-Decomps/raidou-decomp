#include "shared/effect/effModelTrackPoly/decls.h"

dds3HermitianTrackPoly_tag *dds3HermitianTrackPolyCreate(s32 face, s32 split) {
    s32 count = face * split * 2 + 4;
    s32 bytes = count * (sizeof(u128) + sizeof(u32));
    sdfMemHandle_t *handle = sdfMemAllocDirect(bytes + sizeof(dds3HermitianTrackPoly_tag));
    u8 *cursor = (u8 *)sdfMemLock(handle);
    dds3HermitianTrackPoly_tag *data = (dds3HermitianTrackPoly_tag *)(cursor + bytes);

    data->pVertex = (u128 *)cursor;
    cursor += count * sizeof(u128);
    data->blend = 2;
    data->abgr = 0x80808080;
    data->point = count;
    data->count = 0;
    data->current = 2;
    data->split = split;
    data->hMem = handle;
    data->pVC = (u32 *)cursor;
    data->pMat = sdfMatNew();
    sdfMatSetConstant((sdfMat_t *)data->pMat, 1.0f);
    memset(&effModelTrackPolyPrimParm, 0, sizeof(effModelTrackPolyPrimParm));
    effModelTrackPolyPrimParm.attr = 0x4000;
    effModelTrackPolyPrimParm.faceIndex = effModelTrackPolyFaceIndex;
    return data;
}
