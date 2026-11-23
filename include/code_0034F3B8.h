#ifndef CODE_0034F3B8_H
#define CODE_0034F3B8_H

#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "code_r1_001CC1D0.h"

/* A battle's rewards, built by sbtlCallBtlSearchUnitID30 from the battle's
 * drop work and copied by rstSetReward. Each count is clamped by the builder. */
#ifdef VERSION_RAIDOU2
typedef struct rstWork_t {
    /* 0x00 */ s32 money;
    /* 0x04 */ s32 item[8][3]; /* item id, count and a second count added to it */
    /* 0x64 */ s32 unk64;
    /* 0x68 */ s32 exp;               /* Raidou's */
    /* 0x6C */ s32 exCyuusei;         /* loyalty reward, 0 for none */
    /* 0x70 */ s32 stockExp[20];
    /* 0xC0 */ s32 stockCyuusei[20];
    /* 0x110 */ s32 unk110;
    /* 0x114 */ s32 StockIndex0; /* HD name: stock slot of the first party devil */
    /* 0x118 */ s32 StockIndex1; /* HD name */
    /* 0x11C */ u8 unk11C[4];
    /* 0x120 */ u32 unk120; /* bit 0 plays the level up sound */
    /* 0x124 */ s32 drawexp; /* HD names: the amounts counted up on the result screen */
    /* 0x128 */ s32 drawchu;
    /* 0x12C */ s32 drawmon;
} rstWork_t;
#else
typedef struct rstWork_t {
    s32 money;          /* 0..99999 */
    s32 item[8][2];     /* item id and count */
    s32 unk44;
    s32 exp;            /* Raidou's, 0..99999 */
    s32 stockExp[12];   /* the summoned stock demon's, 0..99999 */
    s32 stockUnk7C[12]; /* the summoned stock demon's, 0..100 */
    s32 mag;            /* 0..9999 */
    s32 summonIndex;    /* the stock slot summoned, or -1 */
} rstWork_t;
#endif

extern rstWork_t rstWork;
extern u32 rstData4;
extern dds3ProcessID_t *inputpanel_id;
extern struct itfSprList_t SetCommand_jmsMenuData5;
extern struct itfSprList_t SetCommand_jmsMenuData6;
extern dds3ProcessID_t *g_mainPID3;

/* A key of the position curves rstCalcPosition interpolates (HD rstPositionTable_t). */
typedef struct rstPositionTable_t {
    s32 frame;
    sdfIVector2_t pos;
} rstPositionTable_t;
void rstCalcPosition(s32 frame, const rstPositionTable_t *table, s32 count, sdfIVector2_t *pos);
/* A key of the scale curves rstCalcScale interpolates (HD rstScaleTable_t). */
typedef struct rstScaleTable_t {
    s32 frame;
    sdfVector2_t scale;
} rstScaleTable_t;
void rstCalcScale(s32 frame, const rstScaleTable_t *table, s32 count, sdfVector2_t *scale);
void rstResultLevelUpAnimSub(s32 a_x, s32 a_y, f32 *a_scale, u32 a_blend, s32 a_table, s32 a_step, struct sprHandle_t *a_hSpr,
                             s32 a_layer);

/* Partial layouts of process work read through dds3GetProcessData. */
typedef struct jmsInputPanelWork_t {
    struct sdfMemHandle_t *hMem;
    dds3ProcessID_t *menuPID;
    dds3ProcessID_t *drawPID;
#ifdef VERSION_RAIDOU2
    struct sprHandle_t *hSpr; /* 0x0C */
    s32 step;
    s32 flags; /* 0x14 */
    s32 count;
    s32 unk1C;
    u8 unk20[8];
    f32 unk28;
    s32 unk2C;
    u8 unk30[8];
    u8 unk38; /* 0x38: second fclBackSprite alpha */
    u8 unk39[3];
#else
    u8 unk0C[4];
    u8 step;
    u8 flags;
    u16 count;
    s32 unk14;
#endif
} jmsInputPanelWork_t;

typedef struct rstMainWork_t {
    u8 unk00[4];
    dds3ProcessID_t *giftMsgPID;
    u8 unk08[0x10];
    u8 step; /* 0x18 */
    u8 unk19[5];
    u16 count; /* 0x1E */
    u32 flags;
} rstMainWork_t;

extern const char cmpCalcProcName[];
extern const char campDrawProcName[];
extern const char campUpdateProcName[];
extern const char cmpStrJmsMenuShutdownStart[];
extern const char cmpStrJmsMenuShutdownEnd[];
extern const char jmsStrJmsMenuProcessEndAccept[];


void rstSetReward(const rstWork_t *pReward);
void Release_jmsMenu(jmsInputPanelWork_t *pWork);
void jmsSetFocus(s8 a_bFocus);

/* Partial layout of the process work that rstCallKillProcess releases. */
typedef struct rstProcWork_t {
    struct sdfMemHandle_t *hMem;
    u8 unk04[4];
    struct sdfMemHandle_t *hDelayed;
    s32 unk0C;
    u8 unk10[4];
    dds3ProcessID_t *pid;
} rstProcWork_t;

void rstCallKillProcess(rstProcWork_t *pWork);
void rstResultResultDraw(jmsSeqWork_t *pWork);

typedef struct cmpMemWork_t {
    struct sdfMemHandle_t *handle;
    s32 unk04;
} cmpMemWork_t;
void cmpStartProcessPanelEff(void);

#endif
