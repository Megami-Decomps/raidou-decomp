#include "common.h"
#include "dds3KernelCore.h"
#include "dds3KernelDraw.h"
#include "effStat.h"
#include "effBlur_Filter.h"
#include "code_r1_00226EC8.h"
#include "code_r1_0021CE60.h"

INCLUDE_ASM(const s32, "code_00232D20", effLeaf338);

INCLUDE_ASM(const s32, "code_00232D20", effLeaf339);

INCLUDE_ASM(const s32, "code_00232D20", effGetFlags);

INCLUDE_ASM(const s32, "code_00232D20", effLeaf);

INCLUDE_ASM(const s32, "code_00232D20", effCallStopStat);

INCLUDE_ASM(const s32, "code_00232D20", effStopEffectStop);

INCLUDE_ASM(const s32, "code_00232D20", effFadeInBlur);

INCLUDE_ASM(const s32, "code_00232D20", effCallRestoreStat);

INCLUDE_ASM(const s32, "code_00232D20", effLeaf340);

INCLUDE_ASM(const s32, "code_00232D20", effLeaf341);

void Init_effStat(void) {
    flags_stat = 0;
    effInit();
    proc_stat = dds3StartProcess(Init_effStatStrEffStat, 11010, 0, 0, Process_effStat, NULL, NULL);
}

INCLUDE_ASM(const s32, "code_00232D20", Process_effStat);

INCLUDE_ASM(const s32, "code_00232D20", effLeaf342);

INCLUDE_ASM(const s32, "code_00232D20", effLeaf343);

INCLUDE_ASM(const s32, "code_00232D20", effLeaf344);

void effUpdateBlur(void) {
    f32 blur;

    if (flags_blur & 2) {
        blur = effBlurGetBlurValid() + vel_blur;
        if (blur >= 1.0f) {
            blur = 1.0f;
            flags_blur &= ~2;
        }
        effBlurSetBlurValid(blur);
    }
    else if (flags_blur & 1) {
        blur = effBlurGetBlurValid() - vel_blur;
        if (blur <= 0.0f) {
            blur = 0.0f;
            flags_blur &= ~1;
        }
        effBlurSetBlurValid(blur);
    }
}

INCLUDE_ASM(const s32, "code_00232D20", effStopStat);

INCLUDE_ASM(const s32, "code_00232D20", effRestoreStat);

void effInit(void) {
    u32 i;

    for (i = 0; i < EFFSTAT_QUAKEMAX; i++) {
        quake_list[i].flags = 0;
    }
    flags_quake = 0;
}

void effUpdate(void) {
    effQuakeRequest_t *req = quake_list;
    u32 i;

    for (i = 0; i < EFFSTAT_QUAKEMAX; i++, req++) {
        u16 flags = req->flags;

        if (~flags & 1) {
            continue;
        }
        if (~flags & 4) {
            continue;
        }
        if (flags_quake & 1) {
            continue;
        }
        switch (req->type) {
        case 0:
            break;
        case 1:
            if (++req->count == req->frame) {
                if (flags & 8) {
                    effStop(req);
                }
                req->flags &= 0xFFFB;
                req->flags &= 0xFFF7;
                if (req->flags & 2) {
                    effRemove(req);
                }
            }
            break;
        }
    }
}

effQuakeRequest_t *effRegist(void) {
    u32 i;

    for (i = 0; i < EFFSTAT_QUAKEMAX; i++) {
        if (~quake_list[i].flags & 1) {
            break;
        }
    }
    quake_list[i].flags = 1;
    return &quake_list[i];
}

INCLUDE_ASM(const s32, "code_00232D20", effStart);

void effStartDamp(effQuakeRequest_t *req, s32 amp, s32 frame) {
    req->type = 1;
    req->frame = frame;
    req->amp = amp;
    effStartSignal(req);
}

void effStartSignal(effQuakeRequest_t *req) {
    effQuakeRequest_t *other = quake_list;
    u32 priority = 0;
    u32 i;

    for (i = 0; i < EFFSTAT_QUAKEMAX; i++, other++) {
        u16 flags = other->flags;

        if (~flags & 1) {
            continue;
        }
        if (~flags & 4) {
            continue;
        }
        if (priority < other->priority) {
            priority = other->priority;
        }
        if (flags & 8) {
            other->flags = flags & 0xFFF7;
        }
    }
    req->priority = priority;
    req->count = 0;
    req->flags |= 0xC;
    dds3SetQuake(req->frame, req->amp);
}

void effStop(effQuakeRequest_t *req) {
    if (req->flags & 8) {
        dds3StopQuake();
    }
    req->flags &= 0xFFF3;
}

void effRemove(effQuakeRequest_t *req) {
    if (req->flags & 4) {
        if (req->flags & 8) {
            effStop(req);
        }
    }
    req->flags &= 0xFFFE;
}

INCLUDE_ASM(const s32, "code_00232D20", fclCallDrawSprite);

void fclInitDstEff2Sub(plistHandle_t *list) {
    fileJob_t *job;

    for (job = list->first; job != NULL; job = job->next) {
        fileJobPayload_t *parts = (fileJobPayload_t *)job->id;

        if (parts->type == 24) {
            ((opcHandle_t *)parts->data)->flags |= EFFOPRT_FLAG_IGNORESTOP;
        }
    }
}

INCLUDE_ASM(const s32, "code_00232D20", fclCallSingleMotion);

INCLUDE_ASM(const s32, "code_00232D20", fclLeaf9);

void fclEffectManagerInitSub(void) {
    effLoadFlashTextures();
    effLoadWindTexture();
    effLoadScalyTexture();
    fclBtlDebugSetStatusflgSub();
}

INCLUDE_ASM(const s32, "code_00232D20", fclLeaf10);

INCLUDE_ASM(const s32, "code_00232D20", fclLeaf11);

INCLUDE_ASM(const s32, "code_00232D20", fclBtlDebugSetStatusflgSub);

INCLUDE_RODATA(const s32, "code_00232D20", Init_effStatStrEffStat);

