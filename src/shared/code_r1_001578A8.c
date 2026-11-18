#include "common.h"
#include "dds3KernelDraw.h"
#include "field.h"
#include "mapLbWork.h"
#include "sdfAspect.h"

/* NON_MATCHING: 5 of 262 words match. Same instruction count, scheduling and register allocation differ. */
#ifdef NON_MATCHING
void fldCallSetOt3(u32 color, f32 ox, f32 oy) {
    f32 scale = 1.0f;
    f32 cell = Zoom * 126.0f;
    f32 x;
    f32 y;
    s32 first = 1;
    s32 entryIdx = 0;
    s32 row;
    s32 col;
    s32 n;
    s32 idx;
    mapLbSprite_t *sp;

    if (evtStageTestState2.handle == NULL || fldJmsQuestMapLoad_fldData == NULL || fldCallSetOt3Data == NULL || fldData22 != 0) {
        return;
    }
#ifdef VERSION_RAIDOU1
    eglSetOt(0x2D);
#else
    eglSetOt(0x2E);
#endif
    eglLinkAlpha(0);
    if (fldGlobalBase.amSBlock == 9 && fldGlobalBase.nameU == 0 && fldGlobalBase.cutIdx == 3) {
        eglLinkTest(1, 5, 0xD, 0, 0, 0, 1, 3);
    } else {
        eglLinkTest(1, 5, 2, 0, 0, 0, 1, 3);
    }
    y = oy;
    for (row = 0; row < evtStageTestState2.head->h; row++) {
        x = ox;
        for (col = 0; col < evtStageTestState2.head->w; col++, entryIdx++) {
            if (x + cell < SDF_ASPECT_LEFT || x > SDF_ASPECT_RIGHT || y + cell < 0.0f || y > 448.0f) {
                x += cell;
                continue;
            }
            idx = row * 10 + col;
            sp = fldData5[idx];
            if (fldCallSetOt3Data->count[idx] != 0) {
                if (first) {
                    first = 0;
                    fldCallGrAllocPacketBuff2(evtStageTestState2.entry[entryIdx]);
                } else {
                    fldCallGrAllocPacketBuff3(evtStageTestState2.entry[entryIdx]);
                }
                for (n = 0; n < fldCallSetOt3Data->count[idx]; n++, sp++) {
                    if (sp->tex != 0) {
                        LinkSprite4F(ox + sp->pos[0] * Zoom * scale, oy + sp->pos[1] * Zoom * scale, ox + sp->pos[2] * Zoom * scale, oy + sp->pos[3] * Zoom * scale,
                                     ox + sp->pos[4] * Zoom * scale, oy + sp->pos[5] * Zoom * scale, ox + sp->pos[6] * Zoom * scale, oy + sp->pos[7] * Zoom * scale,
                                     sp->uv[0], sp->uv[1], sp->uv[2], sp->uv[3], sp->uv[4], sp->uv[5], sp->uv[6], sp->uv[7], sp->tex, color, color, color, color, fldTitleMiniTex2);
                    }
                }
            }
            x += cell;
        }
        y += cell;
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_001578A8", fldCallSetOt3);
#endif
