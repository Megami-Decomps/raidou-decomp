#ifndef ITFPANEL_H
#define ITFPANEL_H

#include "common.h"
#include "sdf_resource.h"
#include "interface.h"

typedef struct itfMesShake_t itfMesShake_t;

void DrawTestAlphaPrim(sdfDrawTag_t *pdrawtag);
void itfPnlDraw(itfpanel_t *handle, itfMesShake_t *offset, sdfGrOt_t *pot);
itfpanel_t *itfPnlRegist2(s32 type, saddr_t texhandle);
void itfPnlRelease(itfpanel_t *handle);
void itfPnlSetPosition(itfpanel_t *handle, s32 x0, s32 y0, s32 x1, s32 y1, s32 z);
void itfPnlSetPositionOffset(itfpanel_t *handle, s32 x0, s32 y0, s32 x1, s32 y1, s32 z);
void itfPnlSetColor(itfpanel_t *handle, s32 r, s32 g, s32 b, s32 a);

#endif
