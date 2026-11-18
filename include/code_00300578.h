#ifndef CODE_00300578_H
#define CODE_00300578_H

#include "common.h"
#include "kernel.h"
extern dds3ProcessID_t *g_mainPID;
void titleFunc7(void);
s32 fldCallSetFontColor(f32 arg);
void cmpCallDrawSprList(s32 mode, s32 kind, dds3ProcessID_t *target, u32 flag, s32 type);
void *sysLeaf50(s32 arg);
/* Process data of the cmpStatus process (g_mainPID). */
typedef struct cmpStatusWork_t {
    struct sdfMemHandle_t *hMem; /* 0x00 */
    dds3ProcessID_t *drawPID;    /* 0x04 */
    s32 flags;                   /* 0x08: bit 0 ends the process; sign bit read by cmpGetFlagCallDebug */
    s8 step;                     /* 0x0C */
    s8 step2;                    /* 0x0D */
    u8 unk0E[6];
    s16 count;                   /* 0x14 */
    u8 unk16[0x13D4 - 0x16];
} cmpStatusWork_t;
void Release_cmpStatus(cmpStatusWork_t *wk);
#ifdef VERSION_RAIDOU1
s32 cmpCallGetProcessData2(dds3ProcessID_t *id);
s32 cmpCallGetProcessData3(dds3ProcessID_t *id);
#endif
void cmpCallInitCmpModel(cmpStatusWork_t *wk);
void cmpCallEndCmpModel(cmpStatusWork_t *wk);
void Shutdown_cmpStatus(dds3ProcessID_t *id);

extern s32 nbData8;
extern f32 cmpData6[3];
extern sdfVector4_t cmpStatusModelPos;
extern sdfVector4_t cmpStatusModelBasePos;
extern sdfVector4_t cmpData2;
extern u32 effPktDefaultFaceIndex2[];
extern struct dds3ModelResrc_t *cmpCallPrepareData;
extern s32 (*CmpOnMenuData[])(void);
extern s32 (*Update_cmpItemData[])(void);
extern s32 cmpData26;
extern struct sdfMemHandle_t *fldTitleMiniWork5;
extern u32 cmpCampEFFTESData;
extern struct effHandle_t *fldTitleMiniTex5;

/* nbLeaf71 is empty in retail. */
void nbLeaf69(s32 a, s32 b);
void nbLeaf71(s32 a, s32 b);
void frTestFontInit(void);
void cmpFreeSpriteToGS(void);

#endif
