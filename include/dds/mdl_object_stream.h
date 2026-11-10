#ifndef DDS_MDL_OBJECT_STREAM_H

#define DDS_MDL_OBJECT_STREAM_H
#include "sdf.h"

/* These four bytes are consumed by the same sound-format initializer. */
typedef struct SoundFormat {
    u8 hasAudio;
    u8 stereo;
    u8 loopMode;
    u8 playbackCadenceStep;
} SoundFormat;

typedef SoundFormat SdfStreamParams;

typedef char SdfStreamParams_size_must_be_4[(sizeof(SdfStreamParams) == 4) ? 1 : -1];

/* The model-object allocation ends with its complete embedded stream node. */
typedef struct MdlObj {
    u8 *sourceAddress; /* 0x00: frame data passed to sdfIpuSetDecodeParams */
    sdfMemHandle_t *backingAllocation; /* 0x04: released by mdlDeleteEffectResrc_IPU */
    u8 inUse; /* 0x08: claimed by a model resource item */
    u8 initialized; /* 0x09: stream-node initialization guard */
    u8 pad0A[6];
    s32 sourceSize; /* 0x10 */
    u8 pad14[0x0C];
    sdfIpuDecodeCb_t soundNode; /* 0x20: complete 0x8C embedded node */
} MdlObj;

typedef char MdlObj_size_must_be_0xAC[(sizeof(MdlObj) == 0xAC) ? 1 : -1];

#endif /* DDS_MDL_OBJECT_STREAM_H */
