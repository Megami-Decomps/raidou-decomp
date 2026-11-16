#ifndef TITLEMAIN_H
#define TITLEMAIN_H

#include "common.h"
#include "kernel.h"
#include "sdf_resource.h"
#include "code_r1_001CC1D0.h"

struct effList_t;

/* The title key tables below are 3 (frame, value) pairs. */
typedef struct titleKey_t {
    s32 frame;
    s32 value;
} titleKey_t;

/* Title sprite placement: sprite set index, slot in the set and screen position. */
typedef struct titleSpritePlacement_t {
    s32 spriteIndex;
    s32 slotIndex;
    s32 x;
    s32 y;
} titleSpritePlacement_t;

typedef struct titleKeyTable_t {
    titleKey_t key[3];
} titleKeyTable_t;

/* Allocated by TitleInit as a locked memory handle (0xD0 bytes, 0xE8 in Raidou 2); the first words
 * and the four title sprite slots are the part TitleInit sets up. */
typedef struct titleCursor_t {
    u32 Stat;
    s32 Timer;
    s32 Timer2;
    s32 ItemAlpha[5];
    s32 ScaleTimer;
    s32 Index;
    s32 ConfigIndex;
    s32 LastPush; /* HD has LevelIndex here and LastPush after it */
} titleCursor_t;

typedef struct titleData_t {
    struct sdfMemHandle_t *hMem;
    u32 flag;
    s32 mode;
    jmsSeqHandle_t panel;
    s32 seqUnit;
    struct effList_t *spriteList;
    void *sprite[4];
    s32 Timer;
    s32 SkipTimer;
    s32 WaitTimer;
    s32 introFrame;
    dds3ProcessID_t *PmvPID; /* event viewer process that plays the title movie */
    titleCursor_t Cursor;
    s32 FadeOut;
    s32 MoviePlayWait;
    s32 MovieCounter;
    s32 MovieAlpha; /* 0..128 */
    s32 MovieSkipFlag;
    s32 unkC8;
    s32 AdvSkipFlag;
#ifdef VERSION_RAIDOU2
    u8 unkD0[0x18];
#endif
} titleData_t;

/* evtFunc11's logo animation state. */
typedef struct titleLogoState_t {
    s32 unk00;
    s32 frame0;
    s32 frame1;
    s32 weight[5];
    s32 step;
    s32 phase;
} titleLogoState_t;

#ifdef VERSION_RAIDOU2
typedef char titleData_size_must_be_0xE8[(sizeof(titleData_t) == 0xE8) ? 1 : -1];
#else
typedef char titleData_size_must_be_0xD0[(sizeof(titleData_t) == 0xD0) ? 1 : -1];
#endif

dds3ProcessFunc_t TitleAdvertiseCalcProcess(dds3ProcessID_t *PID);
s32 TitleAdvertiseInit(dds3ProcessID_t *PID);
s32 TitleAtlusLogoCalcProcess(dds3ProcessID_t *PID);
s32 TitleAtlusLogoDrawProcess(dds3ProcessID_t *PID);
dds3ProcessFunc_t TitleConfigCalcProcess(dds3ProcessID_t *PID);
s32 TitleConfigDestroy(dds3ProcessID_t *PID);
s32 TitleDLCInit(dds3ProcessID_t *PID);
s32 TitleDestroy(dds3ProcessID_t *PID);
dds3ProcessFunc_t TitleEndWaitDrawProcess(dds3ProcessID_t *PID);
s32 TitleFirstCalcProcess3(dds3ProcessID_t *PID);
s32 TitleFirstDrawProcess(dds3ProcessID_t *PID);
titleData_t *TitleInit(s32 mode);
void TitleProcessStart(s32 mode);
void TitleGoMain(titleData_t *pData);
s32 TitleAt3Load(titleData_t *pData);
void TitleInitCursor(titleCursor_t *cursor);
void TitleAt3Play(titleData_t *pData);
void evtCallBgmStop(titleData_t *pData);
s32 evtCallStop(titleData_t *pData);
s32 evtCallPlay(s32 MovieNo, s32 Length, s32 FadeFrame, s32 Mode, titleData_t *pData);
s32 TitleProcessCheck(void);
void TitleProcessEnd(void);
void evtFunc11(titleLogoState_t *state, void **sprite);
s32 rstCallGetProcessData30(dds3ProcessID_t *PID);
s32 evtLeaf30(s32 value, s32 low, s32 high);
s32 evtFunc9(s32 frame, const titleKey_t *key, s32 count);
void evtFunc10(s32 slot, s32 value, void **sprite);
void evtLeaf31(s32 slot, s32 value, const f32 *size, const f32 *scale, void **sprite);
void rstFunc(titleData_t *data);
void fldCallElvReset(s32 iMode);
dds3ProcessFunc_t rstCallGetProcessData32(dds3ProcessID_t *PID);
dds3ProcessFunc_t rstCallGetProcessData33(dds3ProcessID_t *PID);
s32 rstCallGetProcessData34(dds3ProcessID_t *PID);

#endif
