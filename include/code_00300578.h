#ifndef CODE_00300578_H
#define CODE_00300578_H

#include "common.h"
#include "kernel.h"
extern dds3ProcessID_t *g_mainPID;
void titleFunc7(void);
s32 fldCallSetFontColor(f32 arg);
void cmpCallDrawSprList(s32 mode, s32 kind, dds3ProcessID_t *target, u32 flag, s32 type);
void sysLeaf50(s32 arg);
/* Process data of the cmpStatus process (g_mainPID). */
typedef struct cmpStatusWork_t {
    u8 unk00[8];
    s32 flags; /* 0x08: sign bit read by cmpGetFlagCallDebug */
} cmpStatusWork_t;

extern s32 nbData8;
extern f32 cmpData6[3];
extern sdfVector4_t cmpStatusModelPos;
extern sdfVector4_t cmpStatusModelBasePos;
extern sdfVector4_t cmpData2;
extern u32 effPktDefaultFaceIndex2[];
extern struct dds3ModelResrc_t *cmpCallPrepareData;
extern void (*CmpOnMenuData[])(void);
extern s32 (*Update_cmpItemData[])(void);
extern s32 cmpData26;
extern struct sdfMemHandle_t *fldTitleMiniWork5;
extern u32 cmpCampEFFTESData;
extern struct effHandle_t *fldTitleMiniTex5;

#endif
