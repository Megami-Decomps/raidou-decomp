#ifndef SDF_GS_MEM_IO_H
#define SDF_GS_MEM_IO_H

#include "common.h"

typedef struct sdfStoreTransfer_t {
    s32 length;
    u32 address;
    void (*callback)(u32 parameter);
    u32 callbackParameter;
} sdfStoreTransfer_t;

#include "sdf_resource.h"

struct sdfPreDrawTag_t;

void sdfLinkLoadImage(sdfDrawTag_t *aDrawTag, sdfGsMemSlice_t *aGsHandle, s32 aDstX, s32 aDstY, s32 aWidth, s32 aHeight, saddr_t aBuffer, s32 (*aAllocator)(s32));

void sdfLinkStoreFrameImage(sdfDrawTag_t *aDrawTag, struct sdfPreDrawTag_t *aPreDrawTag, s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight, s32 aBuffer, s32 aCallback, s32 aCallbackParam, s32 (*aAllocator)(s32));

#endif
