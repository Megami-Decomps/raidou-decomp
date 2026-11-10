#ifndef DDS_EFF_BLUR_H

#define DDS_EFF_BLUR_H
#include "../effect.h"
#include "common.h"

#include "sdf_texture_draw_packet.h"

struct sdfTexHandle_t;

struct sdfDrawTag_t;

typedef struct EffBlurTemplate {
    dds3Effect_PartialMultiBlurParam_t body;
    struct sdfTexHandle_t *texture;
} EffBlurTemplate;

typedef char EffBlurTemplateSizeCheck[sizeof(EffBlurTemplate) == 0x30 ? 1 : -1];

/* One ST/XYZ2 pair in the packed draw payload. */
typedef struct BlurPacketVertex {
    f32 s, t;
    u8 pad08[8];
    s32 x, y;
    u32 depth;
    u16 xyzControl;
    u8 pad1E[2];
} BlurPacketVertex;

typedef struct BlurPacketQuad {
    u32 color[4];
    BlurPacketVertex vertices[4];
} BlurPacketQuad;

typedef struct EffBlurScatterSlot {
    s32 delay;
    f32 angle;
    dds3Effect_Blur_t quad;
} EffBlurScatterSlot;

struct sdfMemHandle_t;

typedef struct EffBlurScatterWork {
    dds3Effect_DistortionBlurParam_t params;
    struct sdfTexHandle_t *texture;
    struct sdfMemHandle_t *allocation;
    EffBlurScatterSlot *slots;
} EffBlurScatterWork;

typedef char EffBlurScatterSlotSizeCheck[sizeof(EffBlurScatterSlot) == 0x30 ? 1 : -1];

typedef char EffBlurScatterWorkSizeCheck[sizeof(EffBlurScatterWork) == 0x38 ? 1 : -1];

typedef struct EffBlurScaleSlot {
    f32 phase;
    f32 angle;
    dds3Effect_Blur_t quad;
} EffBlurScaleSlot;

typedef struct EffBlurScaleWork {
    dds3Effect_RippleBlurParam_t params;
    struct sdfTexHandle_t *texture;
    struct sdfMemHandle_t *allocation;
    EffBlurScaleSlot *slots;
} EffBlurScaleWork;

typedef char EffBlurScaleSlotSizeCheck[sizeof(EffBlurScaleSlot) == 0x30 ? 1 : -1];

typedef char EffBlurScaleWorkSizeCheck[sizeof(EffBlurScaleWork) == 0x38 ? 1 : -1];

void effBlurStepScatterSlotsAndDraw(EffBlurScatterWork *work);

void effAppendBlurRectanglePackets(struct sdfDrawTag_t *list, const dds3Effect_Blur_t *source,
                                   u8 fixedPointCoordinates);

void effDrawBlurSource(const dds3Effect_Blur_t *source, struct sdfTexHandle_t *resource, u8 fixedPointCoordinates);

void effDrawBlurRectangle(const dds3Effect_Blur_t *source);

void effBlurDrawFramebufferQuad(const dds3Effect_Blur_t *source);

void effDrawBlurFixedPointRectangle(EffBlurTemplate *owner);

void effDrawBlurPixelRectWithResource(EffBlurTemplate *owner);

#endif
