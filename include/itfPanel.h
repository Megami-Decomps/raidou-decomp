#ifndef ITFPANEL_H
#define ITFPANEL_H

#include "common.h"
#include "sdf_resource.h"
#include "interface.h"

/* Message window shake: dist is the x/y offset, power the per-axis amplitude
 * that decays over timer frames. */
typedef struct itfMesShake_t {
    s32 dist[2];
    s32 active;
    u8 pad0C[4];
    s32 power[2];
    u32 timer;
    u8 pad1C[4];
} itfMesShake_t;

extern void (*DrawTable_itfPanel[])(itfpanel_t *, sdfDrawTag_t *);

void DrawTestAlphaPrim(sdfDrawTag_t *pdrawtag);
void itfPnlDraw(itfpanel_t *handle, itfMesShake_t *offset, sdfGrOt_t *pot);
void itfLeaf6(s32 *src, s32 *dst);
void itfLeaf7(s32 *dst, s32 *src);
void itfPnlDrawSub(itfpanel_t *handle, itfMesShake_t *offset, sdfDrawTag_t *pdrawtag);
itfpanel_t *itfPnlRegist2(s32 type, saddr_t texhandle);
void itfPnlRelease(itfpanel_t *handle);
void itfPnlSetPosition(itfpanel_t *handle, s32 x0, s32 y0, s32 x1, s32 y1, s32 z);
void itfPnlSetPositionOffset(itfpanel_t *handle, s32 x0, s32 y0, s32 x1, s32 y1, s32 z);
void itfPnlSetColor(itfpanel_t *handle, s32 r, s32 g, s32 b, s32 a);

#endif
