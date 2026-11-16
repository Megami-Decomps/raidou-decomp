#ifndef CODE_R1_001CA8F8_H
#define CODE_R1_001CA8F8_H

#include "common.h"
#include "event.h"
#include "kernel.h"

struct evtViewerObject_t;
struct pmdProcessWork_t;

/* Zero-based selections stored in the camp viewer's primary and secondary
 * option fields. The current menus expose two primary rows and three
 * secondary rows; setters preserve the packed two-bit representation. */
typedef s32 MnuCampPrimaryOptionIndex;
typedef s32 MnuCampSecondaryOptionIndex;

typedef struct pmdTmxData_t {
    s32 x;          /* 0x00 */
    s32 y;          /* 0x04 */
    u8 color[4];    /* 0x08 */
    f32 scaleY;     /* 0x0C */
    f32 scaleX;     /* 0x10 */
    s32 enabled;    /* 0x14 */
    s32 variant;    /* 0x18 */
    s32 preserveAlpha; /* 0x1C: RGB-only alpha-fail drawing */
} pmdTmxData_t;

typedef char CampDisplayDefaults_size_is_20[
    (sizeof(pmdTmxData_t) == 0x20) ? 1 : -1];

void evtHokanTmx(f32 ratio, evtFrameInfo_t *from, evtFrameInfo_t *to, pmdTmxData_t *display);
void evtDrawTexture(sdfTexHandle_t *tex, pmdTmxData_t *display, s32 mask, sdfIVector2_t *maskOfs);

MnuCampSecondaryOptionIndex evtEventSkipStartSub(evtViewerInfo_t *scene);
u32 evtFindInputBranchMessage(evtViewerInfo_t *pInfo);

struct evtViewerObject_t *campResolvePendingValue(evtViewerInfo_t *pInfo, char *objname);
void evtEventFadeOutAllBgm(evtViewerInfo_t *pInfo);
void evtEventRegistBGM(evtViewerInfo_t *pInfo, s32 regbgm_no);
void evtGetValueNewTMXFrame(evtViewerInfo_t *viewer, struct evtViewerObject_t *track, s32 frame, pmdTmxData_t *display);
void evtGetValueNewSLightFrame(evtViewerInfo_t *pInfo, struct evtViewerObject_t *obj, s32 nowframe, pmdSceneLightTable_t *sl, f32 *out_soften, f32 *out_bias);
void evtRenderTexStart(evtViewerInfo_t *pInfo);
s32 mnuCampFindMatchingEntryIndex(struct pmdProcessWork_t *wdata, evtViewerInfo_t *pInfo, s32 pm2nametable_idx);
void mnuCampLinkFontGlyph(evtViewerInfo_t *pInfo);
void evtRenderTexStart(evtViewerInfo_t *pInfo);
struct evtViewerObject_t *mnuCampFindEntryByName(evtViewerInfo_t *scene, const char *name);
void evtGetMessageDataAndTypeFromFrame(evtFrameInfo_t *key, s32 *lowBitsOut, s32 *highBitsOut);

void evtCallAllocRenderTexture(evtViewerInfo_t *viewer);

#endif
