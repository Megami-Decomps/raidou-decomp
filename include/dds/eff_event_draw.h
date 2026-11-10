#ifndef DDS_EFF_EVENT_DRAW_H

#define DDS_EFF_EVENT_DRAW_H
#include "../effect.h"
#include "eff_blur.h"

/* The stored screen record keeps eight bytes beyond the copied blur quad. */
typedef struct EffScreenDrawParams {
    dds3Effect_Blur_t source;
    u8 pad28[8];
} EffScreenDrawParams;

typedef char EffScreenDrawParams_size_must_be_0x30[(sizeof(EffScreenDrawParams) == 0x30) ? 1 : -1];

EffScreenDrawParams *effGetLoadDescA(void);

EffScreenDrawParams *effGetLoadDescD(void);

EffScreenDrawParams *effGetCh73Params(void);

dds3Effect_Filter_t *effEventGetSolidRectangleSetupParams(void);

void effEventSetSolidRectangleParameters(dds3Effect_Filter_t *parameters);

void effEnableFramebufferQuad(void);

void effDisableFramebufferQuad(void);

#endif
