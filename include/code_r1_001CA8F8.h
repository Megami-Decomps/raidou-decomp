#ifndef CODE_R1_001CA8F8_H
#define CODE_R1_001CA8F8_H

#include "common.h"
#include "event.h"
#include "kernel.h"

struct evtViewerObject_t;
struct pmdProcessWork_t;

struct evtViewerObject_t *campResolvePendingValue(evtViewerInfo_t *pInfo, char *objname);
void evtEventFadeOutAllBgm(evtViewerInfo_t *pInfo);
void evtGetValueNewSLightFrame(evtViewerInfo_t *pInfo, struct evtViewerObject_t *obj, s32 nowframe, pmdSceneLightTable_t *sl, f32 *out_soften, f32 *out_bias);
void evtRenderTexStart(evtViewerInfo_t *pInfo);
s32 mnuCampFindMatchingEntryIndex(struct pmdProcessWork_t *wdata, evtViewerInfo_t *pInfo, s32 pm2nametable_idx);
void mnuCampLinkFontGlyph(dds3ProcessID_t *proc);

#endif
