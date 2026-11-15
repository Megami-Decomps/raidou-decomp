#include "common.h"
#include "basic.h"
#include "kernel.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "newdata.h"
#include "field.h"
#include "effect.h"
#include "sdfFrame.h"
#include "fldProcess.h"

extern char dds3DebugLabelB[], dds3DebugLabelG[], dds3DebugLabelR[], dds3DebugFmtInt3[], dds3DebugFmtFloat3[];

typedef struct {
    u32 flags;
    s32 stage;
} fldSequenceController_t;

extern const char fldProcDrawName[];
extern void fldAutoMapSeqSetFlag(u32 flag);
extern effBlurScatterWork_t *hDistortionBlur;
extern void fldEncountClearFlag(u32 flag);
extern s32 fldEncountTestFlag(u32 flag);
extern s32 fldAutoMapSeqTestFlag(u32 flag);
extern void fldAutoMapSeqClearFlag(u32 flag);
extern void fldEncountSetFlag(u32 flag);

typedef struct scrProcessWork_s scrProcessWork_s;

typedef struct dds3FontReq_t dds3FontReq_t;

/* destroy runs when the request leaves the list, draw once per frame while it
 * is still on it. */
typedef struct
{
    void (*destroy)(dds3FontReq_t *req);
    void (*draw)(dds3FontReq_t *req);
} dds3FontReqFunc_t;

struct dds3FontReq_t
{
    dds3FontReq_t *prev;
    dds3FontReq_t *next;
    s32 group;
    dds3FontReqFunc_t *func;
};

extern fldSequenceController_t fldAutoMapSeqFlags;
extern s8 slNewGameBit2208, slNewGameBit2209, slNewGameBit2210, slNewGameBit2211;
extern s8 slNewGameBit2212, slNewGameBit2213, slNewGameBit2323, slNewGameBit162;
extern s32 gFldStart, fldSkyEnable, gfldPlayerHasiCnt;
extern void dds3EffectBlur(const dds3Effect_Blur_t *source);

#define DRAW_FIELD_BLUR(color, mode, angle, zoom) { \
    blur.abgr = (color); \
    blur.blend = (mode); \
    blur.rotate = (angle); \
    blur.scale = (zoom); \
    blur.center[0] = 0; \
    blur.center[1] = 0; \
    blur.xy[0][0] = 0; \
    blur.xy[0][1] = 0; \
    blur.xy[1][0] = 0x200; \
    blur.xy[1][1] = 0x1C0; \
    dds3EffectBlur(&blur); \
}

void fldAutoMapSeqSetFlag(u32 flag);
void fldAutoMapSeqClearFlag(u32 flag);
s32 fldAutoMapSeqTestFlag(u32 flag);
void fldEncountSetFlag(u32 flag);
void fldEncountClearFlag(u32 flag);
s32 fldEncountTestFlag(u32 flag);
extern const char fldProcDrawName[16];
extern s32 gFldStart;

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "field/fldProcess", ProcSequenceTable);

#endif
INCLUDE_ASM(const s32, "field/fldProcess", ProcSequence);

INCLUDE_ASM(const s32, "field/fldProcess", IsFldKeyFree);

void fldAutoMapSeqSetFlag(u32 flag)
{
    fldAutoMapSeqFlags.flags |= flag;
}

void fldAutoMapSeqClearFlag(u32 flag)
{
    fldAutoMapSeqFlags.flags &= ~flag;
}

s32 fldAutoMapSeqTestFlag(u32 flag)
{
    return (fldAutoMapSeqFlags.flags & flag) != 0;
}

void fldEncountSetFlag(u32 flag)
{
    fldEncountFlags |= flag;
}

void fldEncountClearFlag(u32 flag)
{
    fldEncountFlags &= ~flag;
}

s32 fldEncountTestFlag(u32 flag)
{
    return (fldEncountFlags & flag) != 0;
}

INCLUDE_ASM(const s32, "field/fldProcess", fldProcTrigSequence);

INCLUDE_ASM(const s32, "field/fldProcess", fldProcShutDown);

INCLUDE_ASM(const s32, "field/fldProcess", fldCallDelayedResrcCheck);

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "field/fldProcess", ProcSequenceStrBunriHajikiLabel);
#else
INCLUDE_RODATA(const s32, "field/fldProcess", ProcSequenceStrBunriHajikiLabel2);
#endif

INCLUDE_RODATA(const s32, "field/fldProcess", fldStrBunriHajiki3Label);

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "field/fldProcess", jtbl_00447820);
#else
INCLUDE_RODATA(const s32, "field/fldProcess", EveHitTask_fldStrGitaiHajikiLabel);

INCLUDE_RODATA(const s32, "field/fldProcess", ProcSequenceStrGitaiHajiki2Label);

INCLUDE_RODATA(const s32, "field/fldProcess", jtbl_004AA5B0);

INCLUDE_RODATA(const s32, "field/fldProcess", jtbl_004AA6F0);
#endif

INCLUDE_RODATA(const s32, "field/fldProcess", fldStrFldProcSequence);

INCLUDE_RODATA(const s32, "field/fldProcess", fldProcDrawName);

INCLUDE_ASM(const s32, "field/fldProcess", fldTest2);

void fldExitEventSub(void) {
}

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "field/fldProcess", mapReturnFromShop);
#else
INCLUDE_ASM(const s32, "field/fldProcess", fldCallBtlResetRndEncCount);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "field/fldProcess", fldCallAdminiCallSequence);
#else
INCLUDE_ASM(const s32, "field/fldProcess", mapReturnFromRndEncBattle);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "field/fldProcess", mselStartGame);
#else
INCLUDE_ASM(const s32, "field/fldProcess", fldCallStrlen);

INCLUDE_ASM(const s32, "field/fldProcess", mapReturnFromGohmaden);

INCLUDE_ASM(const s32, "field/fldProcess", fldCallBitCheck);
#endif

INCLUDE_ASM(const s32, "field/fldProcess", fldLeaf7);

INCLUDE_ASM(const s32, "field/fldProcess", autoNakamaToRaidou);

INCLUDE_ASM(const s32, "field/fldProcess", mapCallMapNext);

INCLUDE_ASM(const s32, "field/fldProcess", mapCallMap);

INCLUDE_ASM(const s32, "field/fldProcess", fldHardSelect1Sub);

INCLUDE_ASM(const s32, "field/fldProcess", EveHitTask);

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "field/fldProcess", mapCallMapTable);
#else
INCLUDE_RODATA(const s32, "field/fldProcess", mapCallMapTable2);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "field/fldProcess", fldSaveLabel);
#else
INCLUDE_ASM(const s32, "field/fldProcess", EveHitTask_fld);
INCLUDE_SDATA(const s32, "field/fldProcess", ProcSequenceData11);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "field/fldProcess", MskL2Task);
#else
INCLUDE_SDATA(const s32, "field/fldProcess", ProcSequenceData12);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "field/fldProcess", ProcSequenceData6);
#else
INCLUDE_SDATA(const s32, "field/fldProcess", ProcSequenceData13);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "field/fldProcess", ProcSequenceData7);
#else
INCLUDE_SDATA(const s32, "field/fldProcess", ProcSequenceData14);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "field/fldProcess", ProcSequenceData8);

INCLUDE_SDATA(const s32, "field/fldProcess", mapData51);
#else
INCLUDE_SDATA(const s32, "field/fldProcess", mapData169);
#endif

INCLUDE_SDATA(const s32, "field/fldProcess", fldProcShutDownData);

INCLUDE_SDATA(const s32, "field/fldProcess", mapData52);

INCLUDE_SDATA(const s32, "field/fldProcess", mapData53);

INCLUDE_SDATA(const s32, "field/fldProcess", mapData54);

INCLUDE_SDATA(const s32, "field/fldProcess", mapCallMapNextData);

INCLUDE_SDATA(const s32, "field/fldProcess", fldAutoMapSeqFlags);

INCLUDE_SDATA(const s32, "field/fldProcess", nbCallGetIntPara8Data);

#ifdef VERSION_RAIDOU1
u32 fldEncountFlags = 0;

INCLUDE_RODATA(const s32, "field/fldProcess", fldStrBadMafuHajikiLabel);

INCLUDE_RODATA(const s32, "field/fldProcess", fldStrMwallDevilonlyLabel);

INCLUDE_RODATA(const s32, "field/fldProcess", fldStrMap13WarpLabel);

INCLUDE_RODATA(const s32, "field/fldProcess", fldStrRunwarpLabel);

INCLUDE_RODATA(const s32, "field/fldProcess", fldStrHasigoLabel);

INCLUDE_RODATA(const s32, "field/fldProcess", fldStrDexitwarpLabel);

INCLUDE_RODATA(const s32, "field/fldProcess", fldStrElvLabel);

INCLUDE_RODATA(const s32, "field/fldProcess", jtbl_00447B30);

#endif
