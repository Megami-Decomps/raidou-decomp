#include "common.h"
#include "facility.h"
#include "result2.h"
#include "camp.h"
#include "kernel.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "draw3d.h"
#include "kernel_draw.h"
#include "sdfFrame.h"

typedef struct frFontHandle_t frFontHandle_t;

typedef struct frFontRequest_t frFontRequest_t;

extern sdfGrPacketHeader_t *sdf2DMakePrimPacketHeader(sdfGrPacketHeader_t *packet, s32 prim, s32 count, s64 registers, s32 loops);

typedef struct
{
    /* 0x0000 */ u8 unk_00;
    /* 0x0001 */ s8 unk_01;
    /* 0x0002 */ u8 unk_0002[2];
    /* 0x0004 */ s8 unk_04;
    /* 0x0005 */ u8 unk_0005[0xF];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ s8 unk_18;
    /* 0x0019 */ u8 unk_0019[3];
    /* 0x001C */ s32 reserved001C;
    /* 0x0020 */ s32 entryStep;
    /* 0x0024 */ s32 targetPhase;
    /* 0x0028 */ u8 reserved0028[4];
    /* 0x002C */ s32 fadeAlpha;
    /* 0x0030 */ s8 unk_30;
    /* 0x0031 */ s8 unk_31;
    /* 0x0032 */ u8 unk_0032[0x112E];
    /* 0x1160 */ s32 unk_1160;
    /* 0x1164 */ s32 unk_1164;
    /* 0x1168 */ s32 unk_1168;
    /* 0x116C */ s32 unk_116C;
    /* 0x1170 */ s32 unk_1170;
    /* 0x1174 */ s32 unk_1174;
} campInfoWork_t;

typedef struct
{
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 unk_01[0x13];
    /* 0x14 */ void *allocation;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
} cmpInfoWindow_t;

typedef struct
{
    u8 flags;
    u8 unk_01[0x1B];
    s32 unk_1C;
} cmbWork_t;

typedef struct
{
    /* 0x0000 */ u8 flags;
    /* 0x0001 */ u8 unk_0001[0x13];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ u8 unk_0018[0x44B0];
    /* 0x44C8 */ void *unk_44C8;
    /* 0x44CC */ s32 unk_44CC;
    /* 0x44D0 */ s32 unk_44D0;
} rcvWork_t;

typedef struct
{
    /* 0x00 */ s8 id;
    /* 0x01 */ s8 type;
    /* 0x02 */ u8 unk_02[2];
    /* 0x04 */ s32 color;
} cmpHeartsIconTableEntry_t;

typedef struct
{
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 unk_01[3];
    /* 0x04 */ s8 unk_04;
    /* 0x05 */ u8 unk_05[0xF];
    /* 0x14 */ void *allocation;
    union {
        /* 0x18 */ u16 slot[16];
        struct {
            u16 reserved0018[10];
            /* 0x2C */ s32 fadeAlpha;
            /* 0x30 */ s8 unk_30;
            /* 0x31 */ s8 unk_31;
            /* 0x32 */ u8 unk_32[6];
        };
    };
    /* 0x38 */ s8 slotCount;
    /* 0x39 */ s8 slotCursor;
} campStatusPanel_t;

typedef struct
{
    /* 0x0000 */ u8 flags;
    /* 0x0001 */ u8 unk_01[3];
    /* 0x0004 */ s8 unk_04;
    /* 0x0005 */ u8 unk_05[0xF];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ s8 state;
    /* 0x0019 */ u8 unk_19[2];
    /* 0x001B */ u8 unk_1B;
    /* 0x001C */ u8 unk_1C[0x18];
    /* 0x0034 */ s32 unk_34;
    /* 0x0038 */ s32 unk_38;
    /* 0x003C */ u8 unk_3C[0x2A8];
} cmpPanelEff_t;

extern void cmpDrawSpriteFlag(u8 id, s32 flag, s32 arg);
extern void cmpDrawSprite(s32 id, s32 flag, s32 arg);
extern void cmpDrawSprite2(s32 id, s32 flag, s32 arg);
extern void cmpDrawBoxSolid(s32 a, s32 b, s32 c, s32 d, s32 e, s32 color, s32 g, s32 h);
extern void cmpDrawBoxG(s32 a, s32 b, s32 c, s32 d, s32 e, s32 *colors, s32 g, s32 h);

typedef struct
{
    /* 0x00 */ u16 count;
    /* 0x02 */ s8 PartyCnt;
    /* 0x03 */ s8 StockCnt;
    /* 0x04 */ s8 InvalidCnt;
    /* 0x05 */ s8 StockIdx[0x10];
    /* 0x15 */ s8 ListIdx[0x10];
    /* 0x25 */ u8 unk_25[0x31];
} cmpLocalStock_t;

/* The stock work the cursor helpers take: two 0x56-byte rows at the top and a
 * pair of 0x103C-byte records at 0xC0, both selected by the same index. */
typedef struct
{
    /* 0x0000 */ cmpLocalStock_t stock[2];
    /* 0x00AC */ u8 unk_00AC[0x14];
    /* 0x00C0 */ cmpStockInfo_t info[2];
} cmpStockWork_t;

typedef struct
{
    /* 0x0000 */ cmpCursorInfo_t cursor;
    /* 0x0038 */ u8 unk_0038[0x1004];
} cmpListPage_t;

typedef struct
{
    /* 0x0000 */ u8 unk_0000;
    /* 0x0001 */ s8 unk_0001;
    /* 0x0002 */ u8 unk_0002;
    /* 0x0003 */ s8 previousPhase;
    /* 0x0004 */ s8 unk_0004;
    /* 0x0005 */ u8 unk_0005[0xF];
    /* 0x0014 */ void *allocation;
    /* 0x0018 */ s8 stockMessagePhase;
    /* 0x0019 */ u8 unk_0019[0x103F];
    /* 0x1058 */ cmpListPage_t page[3];
    /* 0x410C */ u8 unk_410C[0x22];
    /* 0x412E */ s16 itemCursor;
    /* 0x4130 */ u8 itemScroll;
    /* 0x4131 */ u8 unk_4131[0x2053];
    /* 0x6184 */ cmpLocalStock_t stock[2];
    /* 0x6230 */ u8 unk_6230[0x2078];
    /* 0x82A8 */ u8 drawPhase;
    /* 0x82A9 */ u8 unk_82A9[3];
    /* 0x82AC */ cmpLocalStock_t drawStock;
    /* 0x8302 */ u8 unk_8302;
    /* 0x8303 */ s8 unk_8303;
    /* 0x8304 */ u16 itemId[0xA0];
    /* 0x8444 */ u8 itemMode;
    /* 0x8445 */ u8 statusPhase;
    /* 0x8446 */ u8 reserved8446[2];
    /* 0x8448 */ u8 crossPanel[0x20];
    /* 0x8468 */ s32 unk_8468;
    /* 0x846C */ u8 unk_846C[0x34];
    /* 0x84A0 */ s32 effectStep;
    /* 0x84A4 */ void *dhProcess;
    /* 0x84A8 */ s32 RootTimer;
    /* 0x84AC */ s32 fadeStep;
    /* 0x84B0 */ s32 stockAlpha[16];
    /* 0x84F0 */ s8 stockStep[16];
    /* 0x8500 */ s32 stockDelay[16];
} campWork_t;

typedef struct
{
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 flags2;
    /* 0x02 */ u16 skill;
    /* 0x04 */ u8 unk_04[4];
} datItemEntry_t;

/* datSkillTable viewed from +0x20, which is where retail folds the base for
 * cmpChkSkillBad: it forms datSkillTable + 0x20 and indexes from there, so
 * &entry->badtype off the newdata.h declaration emits a different address.
 * kind and id are the IDB's badtype (+0x24) and basstatus (+0x26). */
typedef struct
{
    /* 0x00 */ u8 unk_00[4];
    /* 0x04 */ u8 kind;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u16 id;
    /* 0x08 */ u8 unk_08[0x14];
} datSkillBadWindow_t;

typedef struct
{
    /* 0x00 */ u8 unk_00[0x20];
    /* 0x20 */ datSkillBadWindow_t bad;
} datSkillBadEntry_t;

extern datSkillBadEntry_t datSkillBadEntries[] FAKEMATCH_ASM_NAME("datSkillTable");

extern void cmpDrawBoxLine(u32 x0, u32 y0, u32 z0, u32 x1, u32 y1, u32 z1,
                           const u32 *colors, s32 surfaceIndex);

typedef struct
{
    /* 0x00 */ s8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ s8 unk_02;
    /* 0x03 */ u8 unk_03[2];
    /* 0x05 */ s8 unk_05;
    /* 0x06 */ u8 unk_06[6];
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ void *allocation;
    /* 0x18 */ void *owner;
} cmpSeqInfo_t;

#include "sdk_libc.h"

typedef struct
{
    /* 0x00000 */ u8 unk_00000;
    /* 0x00001 */ s8 unk_00001;
    /* 0x00002 */ u8 unk_00002;
    /* 0x00003 */ u8 unk_00003;
    /* 0x00004 */ s8 unk_00004;
    /* 0x00005 */ s8 unk_00005;
    /* 0x00006 */ u8 unk_00006[2];
    /* 0x00008 */ s32 unk_00008;
    /* 0x0000C */ u8 unk_0000C[0x10];
    /* 0x0001C */ cmpCursorInfo_t combineCursor;
    /* 0x00054 */ u8 unk_00054[0x40BC];
    /* 0x04110 */ cmbStockSlot_t stockSlot[3];
    /* 0x0411C */ u8 birthPanel[0x123EE];
    /* 0x1650A */ s8 unk_1650A;
    /* 0x1650B */ u8 unk_1650B[5];
    /* 0x16510 */ s32 script;
    /* 0x16514 */ s32 scriptArg;
    /* 0x16518 */ u8 unk_16518[4];
    /* 0x1651C */ void *explainData;
    /* 0x16520 */ u8 unk_16520[0x2C];
    /* 0x1654C */ u32 *unk_1654C[5];
    /* 0x16560 */ u8 unk_16560[0x70];
    /* 0x165D0 */ s32 smoke[3];
    /* 0x165DC */ u8 unk_165DC[0x1064];
    /* 0x17640 */ cmpCursorInfo_t menuCursor;
    /* 0x17678 */ u8 unk_17678[0x3060];
    /* 0x1A6D8 */ s32 texHandle[7];
    /* 0x1A6F4 */ u8 unk_1A6F4[0x24];
    /* 0x1A718 */ u8 *table;
    /* 0x1A71C */ s32 unk_1A71C;
    /* 0x1A720 */ fclRibbon_t *ribbon;
} cmbGbWork_t;

extern sdfGrOt_t sdfGrOtTable[85];

typedef struct
{
    /* 0x0000 */ u8 unk_0000[0x1300];
    /* 0x1300 */ u8 frameTex[0x8E0];
} cmpDrawBank_t;

typedef struct
{
    /* 0x00 */ u8 motionId;
    /* 0x01 */ u8 unk_01[7];
    /* 0x08 */ f32 scale;
    /* 0x0C */ f32 evo[3];
    /* 0x18 */ u8 unk_18[4];
    /* 0x1C */ f32 pos[3];
    /* 0x28 */ u8 unk_28[4];
} cmpModelParam_t;

typedef s32 (*cmpSeqFunc_t)(void);

typedef struct
{
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ u8 unk_08;
    /* 0x09 */ u8 unk_09;
    /* 0x0A */ u8 unk_0A;
    /* 0x0B */ u8 unk_0B;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
} cmpDHWindow_t;

typedef struct
{
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
    /* 0x8 */ f32 z;
    /* 0xC */ f32 w;
} cmpVec4_t;

extern void cmpDrawTriG(const u32 *xs, const u32 *ys, u32 z, const u32 *colors, u32 ot);

typedef struct
{
    /* 0x0000 */ void *allocation;
    /* 0x0004 */ s32 unk_0004;
    /* 0x0008 */ s32 unk_0008;
    /* 0x000C */ s32 unk_000C;
    /* 0x0010 */ u8 unk_0010[0xEC];
    /* 0x00FC */ s32 unk_00FC;
    /* 0x0100 */ s32 unk_0100;
    /* 0x0104 */ s32 unk_0104;
    /* 0x0108 */ s32 unk_0108;
    /* 0x010C */ s32 unk_010C;
    /* 0x0110 */ s32 unk_0110;
    /* 0x0114 */ s32 unk_0114;
    /* 0x0118 */ s32 unk_0118;
} stfUiWork_t;

typedef struct
{
    /* 0x00 */ u16 id;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ u16 unk_04;
} cmpMsgEntry_t;

extern s32 D_00345D88[];

#define D_00345D9C ((void **)(D_00345D88 + 5))

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);
extern s32 D_00345D88[5];

/* Encoded player name and glyph count written by frInitNameData. */
typedef struct {
    u8 reserved00[0x6604];
    u8 nameBytes[0x50];
    u8 nameLength;
} cmpSavedNameView_t;

extern const char *cmpMenuStrings[] FAKEMATCH_ASM_NAME("cmpMenuTextTable");

/* The first sprite ID uses a localized live-value constraint; see the
 * matching note for the paired ordinary and nonvolatile controls. */
struct campCrossPanel_t;

typedef struct
{
    /* 0x00 */ u8 unk_00[0x14];
    /* 0x14 */ void *allocation;
    /* 0x18 */ s8 unk_18;
    /* 0x19 */ u8 unk_19[3];
    /* 0x1C */ cmpLocalStock_t *stock;
} cmpStockSeqInfo_t;

typedef struct
{
    /* 0x0000 */ u8 unk_0000[0x116C];
    /* 0x116C */ s32 texHandle[3];
} cmpDhWork_t;

extern campInfoWork_t *volatile campDhCalcStateLive FAKEMATCH_ASM_NAME("campDhCalcState");
extern campWork_t *volatile campWorkLive FAKEMATCH_ASM_NAME("campWork");

typedef struct
{
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 unk_01[0x1B];
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ s32 entryStep;
    /* 0x24 */ s32 targetPhase;
    /* 0x28 */ u8 reserved28[4];
    /* 0x2C */ s32 fadeAlpha;
    /* 0x30 */ s8 unk_30;
    /* 0x31 */ s8 iconIndex;
    /* 0x32 */ u8 reserved32[0x1146];
    /* 0x1178 */ s32 cursorAlpha[64];
} campDhWork_t;

typedef struct
{
    /* 0x00 */ s8 unk_00;
    /* 0x01 */ s8 unk_01;
    /* 0x02 */ u8 unk_02[2];
    /* 0x04 */ u32 color;
    /* 0x08 */ s8 mode;
    /* 0x09 */ s8 direction;
    /* 0x0A */ u8 unk_0A[2];
    /* 0x0C */ s32 directionTimer;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ f32 unk_18;
    /* 0x1C */ f32 unk_1C;
    /* 0x20 */ f32 unk_20;
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
} campDhEntry_t;

extern campDhWork_t *volatile campDhBackgroundWork FAKEMATCH_ASM_NAME("campDhDrawWork");

typedef struct
{
    /* 0x00 */ s32 visible;
    /* 0x04 */ s32 alpha;
    /* 0x08 */ s8 step;
    /* 0x09 */ s8 id;
    /* 0x0A */ u8 pad[2];
    /* 0x0C */ s32 fade;
    /* 0x10 */ s32 fadeStep;
} campDhIconGauge_t;

/* The Hearts panel keeps the pre-selection equipment byte and a complete
 * player unit snapshot, restored after the confirmation message. */
typedef struct
{
    u8 unk_00[0x32];
    s8 equipHearts;
    u8 unk_33;
    datUnitWork_t unit;
} campDhSavedUnit_t;

/* campDhUpdateWork, as this unit's animation and icon code reads it. The two
 * were separate structs whose 0x28-byte prefixes agreed, so they are one here.
 * A third view, campStatusPanel_t *campDhUpdateWork, is declared above: the
 * alias is what lets both spellings of the one object coexist, and unifying
 * them is type recovery on the real allocation, not a rename. */
typedef struct
{
    /* 0x0000 */ u8 unk_0000[0x1C];
    /* 0x001C */ s32 unk_1C;
    /* 0x0020 */ s32 unk_20;
    /* 0x0024 */ s32 unk_24;
    /* 0x0028 */ s32 unk_28;
    /* 0x002C */ u8 unk_002C[0x114C];
    /* 0x1178 */ s32 iconTimer[64];
} campDhTimerWork_t;

extern campDhTimerWork_t *campDhTimerWork FAKEMATCH_ASM_NAME("campDhUpdateWork");

typedef struct
{
    u8 unk_0000[0x1278];
    campDhEntry_t entries[25];
} campDhEntryWork_t;

extern campDhEntryWork_t *campDhEntryWork FAKEMATCH_ASM_NAME("campDhUpdateWork");

#include "font.h"

extern s16 cmpHeartsMasterPhase FAKEMATCH_ASM_NAME("rstHeartsMasterPhase");
extern datUnitWork_t *volatile cmpStatusBirthLive FAKEMATCH_ASM_NAME("cmbBirthPanel");
extern volatile s32 cmpGaugeAnimLive[] FAKEMATCH_ASM_NAME("cmpStatusAnimCount");
extern volatile s32 cmpStatusAnimLive[] FAKEMATCH_ASM_NAME("cmpStatusAnimCount");
extern volatile s32 cmpSkillAnimLive[] FAKEMATCH_ASM_NAME("cmpStatusAnimCount");

typedef struct
{
    /* 0x00 */ u8 unk_00[0x10];
} cmpModelLink_t;

void cmpExpandColor(u64 *dst, u32 color);
extern void sdfDrawTagInit(sdfDrawTag_t *);
extern void sdfDrawTagLink(sdfDrawTag_t *, addr_t);
void cmpDrawSpriteFlag(u8 id, s32 flag, s32 ot);
void cmpDrawSprite(s32 id, s32 flag, s32 ot);
void cmpDrawSprite2(s32 id, s32 flag, s32 ot);
void cmpDrawSpriteIndexed(s32 id, s32 ot);
void cmpDrawSpriteIndexed2(s32 id, s32 ot);
void uiDrawUniformRgbRange(const u32 *xs, const u32 *ys, u32 z, u32 color, u32 ot);
void cmpDrawTriG(const u32 *xs, const u32 *ys, u32 z, const u32 *colors, u32 ot);
void cmpDrawBoxSolid(s32 a, s32 b, s32 c, s32 d, s32 e, s32 color, s32 g, s32 h);
void cmpDrawPanelEffCell(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);

void cmpDrawBoxLine(u32 x0, u32 y0, u32 z0, u32 x1, u32 y1, u32 z1,
                    const u32 *colors, s32 surfaceIndex);

void cmpDraw3DPoly(const f32 *first, const f32 *second, const f32 *third,
                   const u32 *colors, s32 unused, sdfMat_t *asset, s32 surfaceIndex);

typedef struct
{
    s32 x;
    s32 y;
    s32 z;
    u32 color;
} cmpPrintControl_t;

typedef struct campCrossPanel_t
{
    /* 0x00 */ void (*func)(void *, void *, s32, s32, s32, s32);
    /* 0x04 */ u8 pos[0x14];
    /* 0x18 */ cmpPadStopWork_t *cursor;
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ s32 alpha;
    /* 0x24 */ s8 flag;
    /* 0x25 */ u8 unk_25[7];
} campCrossPanel_t;

typedef struct {
    cmpCursorInfo_t cursor;
    u8 reserved38[4];
    cmpCursorSlot_t slot[256][2];
} cmpCursorExtended_t;

typedef struct
{
    /* 0x00 */ s32 pos;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 start;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 target;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 width;
    /* 0x1C */ u8 reflected;
    /* 0x20 */ s32 total;
    /* 0x24 */ s32 left;
    /* 0x28 */ s32 unk_28;
} cmpPanelEffAnim_t;

typedef struct
{
    /* 0x00 */ u8 state;
    /* 0x01 */ u8 step;
    /* 0x02 */ u8 delay;
    /* 0x03 */ u8 kind;
    /* 0x04 */ u32 color4;
    /* 0x08 */ u32 color8;
    /* 0x0C */ s32 x;
    /* 0x10 */ s32 y;
    /* 0x14 */ s32 z;
    /* 0x18 */ u16 width;
    /* 0x1A */ u16 height;
    /* 0x1C */ s32 frame;
    /* 0x20 */ s32 value;
    /* 0x24 */ cmpPanelEffAnim_t anim[16];
} cmpPanelEffState_t;

typedef struct
{
    /* 0x0000 */ u8 unk_0000[8];
    /* 0x0008 */ u8 state;
    /* 0x0009 */ u8 step;
    /* 0x000A */ u8 delay;
    /* 0x000B */ u8 kind;
    /* 0x000C */ u8 unk_000C[0x2D8];
} cmpPanelEffView_t;

extern cmpPanelEff_t *volatile cmpPanelEffLive FAKEMATCH_ASM_NAME("cmpPanelEffList");

typedef struct
{
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t center;
    f32 r;
    f32 h;
    f32 scl;
    void *hModel;
} cmpBadEffectParams_t;

extern void *hCAMPBadEffect;

#include "motion.h"

extern datSkill_t attr[];

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

void *memset(void *dst, s32 value, u32 size);

void sdf3DPrimLinkMat(sdfDrawTag_t *packetList, sdfMat_t *asset,
                              s32 (*allocatePacket)(s32));

/* Append standard packets to a DMA packet list. A null allocator selects the
   default packet allocator. Asset packets reference the selected draw entry. */
void sdf3DPrimLinkResetTm(sdfDrawTag_t *packetList,
                              s32 (*allocatePacket)(s32));

/* Build the serialized VIF attribute stream and return its word cursor. */
u32 *sdf3DPrimMakePoly(sdf3DPrimPolyParm_t *request);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);

#include "dds/sdf_packet_header.h"
#include "dds/eff.h"
#include "dds/eff_slot_phase.h"
#include "dds/eff_resource_slots.h"
#include "dds/eff_resource_records.h"
#include "dds/itf_grid_status_records.h"

extern void *effGetSlotWorkOrOverride(EffectSlotSet *, s32);
extern EffectSlotSet *effUpdateTimedStates(EffectSlotSet *, u32, void *);

/* Four words filled together; their corner/channel interpretation is unknown. */
typedef struct UiQuadWords {
    u32 unk00[4];
} UiQuadWords; // 0x10

extern s32 sdf2DGetPrimPacketSize(s32 registerCount, s32 loopCount);
extern u8 *sdf2DGetPrimPtr(SdfDrawPacket *packet);
extern void effDrawSpriteWithWorkCornerColors(s32 x, s32 y, s32 z, s32 flags, EffectSlotSet *set, s32 slot, BdWork *entry, s32 layer);
void itfDrawGridWithResolvedSlot(s32 offsetX, s32 offsetY, s32 z, s32 drawFlags, EffectSlotSet *resource, s32 resourceIndex, s32 surfaceIndex);
void itfUpdateGridSlotDescription(EffectSlotSet *resource, s32 resourceIndex);
s32 itfGridLookupValueOrDefault(EffectSlotSet *resource, s32 resourceIndex);
void itfSetGridEntryQuantizedAndRefresh(EffectSlotSet *resource, s32 resourceIndex, s32 x, s32 y, s32 width, s32 height);
EffSlotPhaseStep itfGridApplySqrtBoundsAndColorScale(BdWork *rectangle, BdWork *out, EffTimedState *timedState);
EffSlotPhaseStep itfGridApplyQuadraticZoomBoundsAndFadeAlpha(BdWork *rectangle, BdWork *out, EffTimedState *timedState);
EffSlotPhaseStep itfGridApplyTimedBoundsAndAlpha(BdWork *rectangle, BdWork *out, EffTimedState *timedState);
EffSlotPhaseStep itfGridApplyThresholdBoundsAndAlpha(BdWork *rectangle, BdWork *out, EffTimedState *timedState);
EffSlotPhaseStep itfGridInterpolateCornerColorsToTarget(BdWork *rectangle, BdWork *out, EffTimedState *timedState);
EffSlotPhaseStep itfGridApplyLinearZoomBoundsAndFadeAlpha(BdWork *rectangle, BdWork *out, EffTimedState *timedState);
EffSlotPhaseStep itfGridApplyLinearBoundsAndColorScale(BdWork *rectangle, BdWork *out, EffTimedState *timedState);
void sdfSubmitGsTestOneRegisterPacket(u64 data, u32 kind);
void uiDrawUniformColorRect(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 surfaceIndex);
void uiDrawActiveSurfaceRegion(s32 surfaceIndex);
void uiDrawTexturedSurfaceAtFarDepth(s32 surface);

/* Resolve the indexed render entry before applying position, depth, and draw flags. */
void itfDrawGridWithResolvedSlot(s32 offsetX, s32 offsetY, s32 z, s32 drawFlags, EffectSlotSet *resource, s32 resourceIndex, s32 surfaceIndex) {
    void *renderEntry = effGetSlotWorkOrOverride(resource, resourceIndex);
    effDrawSpriteWithWorkCornerColors(offsetX, offsetY, z, drawFlags, resource, resourceIndex, (BdWork *)renderEntry,
                  surfaceIndex);
}

/* Update the indexed slot's description countdown and carry its active state forward. */
void itfUpdateGridSlotDescription(EffectSlotSet *resource, s32 resourceIndex) {
    BdWork *base = &resource->workEntries[resourceIndex];
    const u32 timedByteOffset = (resourceIndex + base->slotOffset) * sizeof(BdWork);
    BdWork *timed = (BdWork *)(timedByteOffset + (u32)resource->workEntries);
    BdWork *previous = (BdWork *)effGetSlotWorkOrOverride(resource, resourceIndex + base->slotOffset);
    BdWork *next;
    u32 advance = 0;

    if (timed->remainingDescriptionUpdates == 0) {
        timed->remainingDescriptionUpdates =
            resource->descriptions[resourceIndex + base->slotOffset].descriptionUpdateDelay;
        if ((u32)(resourceIndex + timed->slotOffset + 1) < resource->count) {
            const s32 nextFlags = resource->descriptions[resourceIndex + base->slotOffset + 1].flags &
                                  EFF_SLOT_DESCRIPTION_SEQUENCE_CONTINUATION;
            const u32 shouldAdvance = nextFlags > 0;
            advance = shouldAdvance;
        }
        if (advance == 1) {
            base->slotOffset++;
        } else {
            if (base->slotOffset == 0) {
                return;
            }
            base->slotOffset = 0;
        }
        effInitializeSlotWorkFromDescription(resource, resourceIndex + base->slotOffset, previous);
        next = (BdWork *)effGetSlotWorkOrOverride(resource, resourceIndex + base->slotOffset);
        next->states[0].flags = previous->states[0].flags;
        next->states[0].source = previous->states[0].source;
        next->states[0].value = previous->states[0].value;
    } else {
        timed->remainingDescriptionUpdates--;
    }
}

/* Resolve an entry by key, falling back to the object's stored value. */
s32 itfGridLookupValueOrDefault(EffectSlotSet *resource, s32 resourceIndex) {
    BdWork *entry = (BdWork *)effGetSlotWorkOrOverride(resource, resourceIndex);
    s32 result;

    if (entry->states[0].delay == 0) {
        itfUpdateGridSlotDescription(resource, resourceIndex);
    }
    result = (s32)effUpdateTimedStates(resource, (u32)resourceIndex, entry);
    if (result == 0) {
        result = resource->defaultValue;
    }
    return result;
}

void itfSetGridEntryQuantizedAndRefresh(EffectSlotSet *resource, s32 resourceIndex, s32 x, s32 y, s32 width, s32 height) {
    EffectSlotDescription *entry = &resource->descriptions[resourceIndex];
    void *record = effGetSlotWorkOrOverride(resource, resourceIndex);

    entry->bounds[0] = x >> 4;
    entry->bounds[1] = y >> 3;
    entry->bounds[2] = width >> 4;
    entry->bounds[3] = height >> 3;
    effInitializeSlotWorkFromDescription(resource, resourceIndex, record);
}

INCLUDE_ASM(const s32, "code_002CAFD8", itfGridSetQuantizedBounds);

INCLUDE_ASM(const s32, "code_002CAFD8", itfGridSetBounds);

INCLUDE_ASM(const s32, "code_002CAFD8", itfGridCopyEntryQuad);

INCLUDE_ASM(const s32, "code_002CAFD8", cmpLeaf34);

EffSlotPhaseStep itfGridApplySqrtBoundsAndColorScale(BdWork *rectangle, BdWork *out, EffTimedState *timedState) {
    GridAngleTable *table;
    s32 deltas[2];
    EffectSlotGeometry *geometry = &out->geometry;
    s32 *dimensions = geometry->bounds;
    u32 *sourceColor;
    u32 *destColor;
    s32 factor;
    s32 i = 0;

    table = (GridAngleTable *)timedState->source->status;
    deltas[0] = (rectangle->bounds.grid.quantizedBounds[2] - rectangle->bounds.grid.quantizedBounds[0]) << 4;
    deltas[1] = (rectangle->bounds.grid.quantizedBounds[3] - rectangle->bounds.grid.quantizedBounds[1]) << 3;
    for (; i < 2; i++) {
        s32 delta = deltas[i];
        s32 scaled = (s32)(fsqrtf((f32)delta) * (f32)timedState->value * (1.0f / 65536.0f));
        if (delta > 0) {
            dimensions[i] = delta - scaled * scaled;
        } else {
            dimensions[i] = scaled * scaled + delta;
        }
    }
    if (table->mirrored != 0) {
        factor = EFF_SLOT_PHASE_ONE - timedState->value;
    } else {
        factor = timedState->value;
    }
    sourceColor = rectangle->savedColors;
    destColor = geometry->cornerColors;
    for (i = 0; i < 4; i++, sourceColor++, destColor++) {
        u32 color = *sourceColor;
        s32 alpha = *(u8 *)sourceColor;
        *destColor = (color & ~0xFF) | (alpha * factor / EFF_SLOT_PHASE_ONE);
    }
    return EFF_SLOT_PHASE_ONE / table->divisor;
}

/* Apply the ZOOM_01 easing to the adjustment bounds and fade their alpha. */
EffSlotPhaseStep itfGridApplyQuadraticZoomBoundsAndFadeAlpha(BdWork *rectangle, BdWork *out, EffTimedState *timedState) {
    GridAngleTable *table;
    s32 squares[2];
    s32 deltas[2];
    s32 previous[2];
    s32 scaledWidth;
    s32 scaledHeight;
    s32 widthAdjustment;
    s32 heightAdjustment;
    u32 *sourceColor;
    u32 *destColor;
    s32 colorMask;
    s32 fractionalMask;
    s32 i;

    table = (GridAngleTable *)timedState->source->status;
    colorMask = -0x100;
    fractionalMask = 0xFFFF;
    deltas[0] = table->divisor << 4;
    deltas[1] = (((table->mirrored << 12) / 640) * rectangle->sourceHeight) / rectangle->sourceWidth;
    previous[0] = out->geometry.bounds[0];
    previous[1] = out->geometry.bounds[1];
    scaledWidth = (s32)(fsqrtf((f32)deltas[0]) * (f32)timedState->value * (1.0f / 65536.0f));
    squares[0] = scaledWidth * scaledWidth;
    widthAdjustment = -((deltas[0] - squares[0]) / 2);
    scaledHeight = (s32)(fsqrtf((f32)deltas[1]) * (f32)timedState->value * (1.0f / 65536.0f));
    squares[1] = scaledHeight * scaledHeight;
    heightAdjustment = -((deltas[1] - squares[1]) / 2);

    out->geometry.bounds[0] = widthAdjustment;
    out->geometry.bounds[2] += (previous[0] - widthAdjustment) * 2;
    out->geometry.bounds[1] = heightAdjustment;
    out->geometry.bounds[3] += (previous[1] - heightAdjustment) * 2;

    sourceColor = rectangle->savedColors;
    destColor = out->geometry.cornerColors;
    i = 3;

    for (; i >= 0; i--, sourceColor++, destColor++) {
        u32 color = *sourceColor;
        s32 alpha = *(u8 *)sourceColor;
        s32 product = alpha * timedState->value;
        s32 negative = 0;

        if (product < 0) {
            negative++;
        }
        *destColor = (color & colorMask) | ((product + negative * fractionalMask) >> 16);
    }
    return EFF_SLOT_PHASE_ONE / table->cycleDivisor;
}

/* Shrink the grid bounds and apply the three-phase flush envelope to corner alpha. */
EffSlotPhaseStep itfGridApplyTimedBoundsAndAlpha(BdWork *rectangle, BdWork *out, EffTimedState *timedState) {
    GridFlushTable *table;
    s32 phases[3];
    s32 deltas[2];
    EffectSlotGeometry *geometry = &out->geometry;
    s32 *dimensionOut;
    s32 totalDuration;
    s32 numerator;
    s32 denominator;
    u32 *sourceColor;
    u32 *destColor;
    s32 i = 0;

    table = (GridFlushTable *)timedState->source->status;
    totalDuration = table->inDuration;
    totalDuration += table->outDuration;
    totalDuration += table->waitDuration;
    deltas[0] = (rectangle->bounds.grid.quantizedBounds[2] - rectangle->bounds.grid.quantizedBounds[0]) << 4;
    deltas[1] = (rectangle->bounds.grid.quantizedBounds[3] - rectangle->bounds.grid.quantizedBounds[1]) << 3;
    dimensionOut = geometry->bounds;
    {
        s32 fractionalMask = 0xFFFF;

        for (; i < 2; i++) {
            s32 delta = deltas[i];
            s32 magnitude = delta < 0 ? -delta : delta;
            s32 product = magnitude * timedState->value;
            s32 negative = 0;
            s32 scaled;

            if (product < 0) {
                negative++;
            }
            /* Signed fixed-point division rounds toward zero. */
            scaled = (product + negative * fractionalMask) >> 16;
            if (delta > 0) {
                dimensionOut[i] = delta - scaled;
            } else {
                dimensionOut[i] = delta + scaled;
            }
        }
    }

    phases[0] = (table->inDuration << 16) / totalDuration;
    phases[1] = (table->waitDuration << 16) / totalDuration;
    phases[2] = (table->outDuration << 16) / totalDuration;

    if (phases[1] + phases[2] < timedState->value) {
        numerator = phases[0] - (timedState->value - (phases[1] + phases[2]));
        denominator = phases[0];
    } else if (phases[2] < timedState->value) {
        destColor = geometry->cornerColors;
        sourceColor = rectangle->savedColors;
        for (i = 3; i >= 0; i--, sourceColor++, destColor++) {
            *destColor = *sourceColor;
        }
        return EFF_SLOT_PHASE_ONE / totalDuration;
    } else {
        numerator = timedState->value;
        denominator = phases[2];
    }

    destColor = geometry->cornerColors;
    sourceColor = rectangle->savedColors;
    {
        s32 colorMask = -0x100;

        for (i = 3; i >= 0; i--, sourceColor++, destColor++) {
            u32 color = *sourceColor;
            s32 lowByte = *(u8 *)sourceColor;

            *destColor = (color & colorMask) | (lowByte * numerator / denominator);
        }
    }
    return EFF_SLOT_PHASE_ONE / totalDuration;
}

EffSlotPhaseStep itfGridApplyThresholdBoundsAndAlpha(BdWork *rectangle, BdWork *out, EffTimedState *timedState) {
    GridAngleTable *table;
    s32 deltas[2];
    EffectSlotGeometry *geometry = &out->geometry;
    s32 *dimensions = geometry->bounds;
    u32 *sourceColor;
    u32 *destColor;
    s32 factor;
    s32 i = 0;

    table = (GridAngleTable *)timedState->source->status;
    deltas[0] = (rectangle->bounds.grid.quantizedBounds[2] - rectangle->bounds.grid.quantizedBounds[0]) << 4;
    deltas[1] = (rectangle->bounds.grid.quantizedBounds[3] - rectangle->bounds.grid.quantizedBounds[1]) << 3;
    for (; i < 2; i++) {
        s32 delta = deltas[i];
        s32 magnitude = delta < 0 ? -delta : delta;
        s32 amount = magnitude * timedState->value / EFF_SLOT_PHASE_ONE;
        if (delta > 0) {
            dimensions[i] = delta - amount;
        } else {
            dimensions[i] = delta + amount;
        }
    }
    factor = ((100 - table->mirrored) << 16) / 100;
    destColor = geometry->cornerColors;
    sourceColor = rectangle->savedColors;
    for (i = 0; i < 4; i++, destColor++, sourceColor++) {
        if (timedState->value < factor) {
            u32 color = *sourceColor;
            s32 alpha = *(u8 *)sourceColor;
            *destColor = (color & ~0xFF) | (alpha * timedState->value / factor);
        } else {
            *destColor = *sourceColor;
        }
    }
    return EFF_SLOT_PHASE_ONE / table->divisor;
}

INCLUDE_ASM(const s32, "code_002CAFD8", itfUpdateAngleAndGetCycleStep);

/* Blend each saved corner color toward the table's target color. */
EffSlotPhaseStep itfGridInterpolateCornerColorsToTarget(BdWork *rectangle, BdWork *out, EffTimedState *timedState) {
    GridColorTargetTable *table = (GridColorTargetTable *)timedState->source->status;
    u32 *destColor = out->geometry.cornerColors;
    u32 *sourceColor = rectangle->savedColors;
    s32 i;

    for (i = 3; i >= 0; i--, sourceColor++, destColor++) {
        u32 color = *sourceColor;
        s32 alpha = (color & 0xFF000000) >> 24;
        s32 blue = (color & 0xFF0000) >> 16;
        s32 green = (color & 0xFF00) >> 8;
        s32 red = *(u8 *)sourceColor;
        s32 alphaTarget = table->alpha;

        blue = (blue + (table->blue - blue) * timedState->value / EFF_SLOT_PHASE_ONE) & 0xFF;
        green = (green + (table->green - green) * timedState->value / EFF_SLOT_PHASE_ONE) & 0xFF;
        red = (red + (table->red - red) * timedState->value / EFF_SLOT_PHASE_ONE) & 0xFF;
        *destColor = ((alpha + (alphaTarget - alpha) * timedState->value / EFF_SLOT_PHASE_ONE) << 24) | (blue << 16)
            | (green << 8) | red;
    }
    return EFF_SLOT_PHASE_ONE / table->divisor;
}

/* Apply linear ZOOM easing to the adjustment bounds and fade their alpha. */
EffSlotPhaseStep itfGridApplyLinearZoomBoundsAndFadeAlpha(BdWork *rectangle, BdWork *out, EffTimedState *timedState) {
    GridAngleTable *table;
    s32 scaled[2];
    s32 deltas[2];
    s32 previous[2];
    s32 scaledWidth;
    s32 scaledHeight;
    s32 widthAdjustment;
    s32 heightAdjustment;
    s32 angle;
    u32 *sourceColor;
    u32 *destColor;
    s32 colorMask;
    s32 fractionalMask;
    s32 i;

    table = (GridAngleTable *)timedState->source->status;
    colorMask = -0x100;
    fractionalMask = 0xFFFF;
    deltas[0] = table->divisor << 4;
    deltas[1] = (((table->mirrored << 12) / 640) * rectangle->sourceHeight) / rectangle->sourceWidth;
    angle = timedState->value;
    previous[0] = out->geometry.bounds[0];
    previous[1] = out->geometry.bounds[1];
    scaledWidth = deltas[0] * angle;
    if (scaledWidth < 0) {
        scaledWidth += 0xFFFF;
    }
    scaled[0] = scaledWidth >> 16;
    widthAdjustment = -((deltas[0] - scaled[0]) / 2);
    out->geometry.bounds[0] = widthAdjustment;
    out->geometry.bounds[2] += (previous[0] - widthAdjustment) * 2;

    scaledHeight = deltas[1] * angle;
    if (scaledHeight < 0) {
        scaledHeight += 0xFFFF;
    }
    scaled[1] = scaledHeight >> 16;
    heightAdjustment = -((deltas[1] - scaled[1]) / 2);
    out->geometry.bounds[1] = heightAdjustment;
    out->geometry.bounds[3] += (previous[1] - heightAdjustment) * 2;

    sourceColor = rectangle->savedColors;
    destColor = out->geometry.cornerColors;
    i = 3;
    for (; i >= 0; i--, sourceColor++, destColor++) {
        u32 color = *sourceColor;
        s32 alpha = *(u8 *)sourceColor;
        s32 colorProduct = alpha * timedState->value;
        s32 negative = 0;

        if (colorProduct < 0) {
            negative++;
        }
        *destColor = (color & colorMask) | ((colorProduct + negative * fractionalMask) >> 16);
    }
    return EFF_SLOT_PHASE_ONE / table->cycleDivisor;
}

EffSlotPhaseStep itfGridApplyLinearBoundsAndColorScale(BdWork *rectangle, BdWork *out, EffTimedState *timedState) {
    GridAngleTable *table;
    s32 deltas[2];
    EffectSlotGeometry *geometry = &out->geometry;
    s32 *dimensions = geometry->bounds;
    u32 *sourceColor;
    u32 *destColor;
    s32 factor;
    s32 i = 0;

    table = (GridAngleTable *)timedState->source->status;
    deltas[0] = (rectangle->bounds.grid.quantizedBounds[2] - rectangle->bounds.grid.quantizedBounds[0]) << 4;
    deltas[1] = (rectangle->bounds.grid.quantizedBounds[3] - rectangle->bounds.grid.quantizedBounds[1]) << 3;
    for (; i < 2; i++) {
        s32 delta = deltas[i];
        s32 magnitude = delta < 0 ? -delta : delta;
        s32 amount = magnitude * timedState->value / EFF_SLOT_PHASE_ONE;
        if (delta > 0) {
            dimensions[i] = delta - amount;
        } else {
            dimensions[i] = delta + amount;
        }
    }
    if (table->mirrored != 0) {
        factor = EFF_SLOT_PHASE_ONE - timedState->value;
    } else {
        factor = timedState->value;
    }
    sourceColor = rectangle->savedColors;
    destColor = geometry->cornerColors;
    for (i = 0; i < 4; i++, sourceColor++, destColor++) {
        u32 color = *sourceColor;
        s32 alpha = *(u8 *)sourceColor;
        *destColor = (color & ~0xFF) | (alpha * factor / EFF_SLOT_PHASE_ONE);
    }
    return EFF_SLOT_PHASE_ONE / table->divisor;
}

INCLUDE_ASM(const s32, "code_002CAFD8", cmpCallMemset);

INCLUDE_ASM(const s32, "code_002CAFD8", cmpLeaf35);

INCLUDE_ASM(const s32, "code_002CAFD8", cmpLeaf36);

INCLUDE_ASM(const s32, "code_002CAFD8", cmpLeaf37);

INCLUDE_ASM(const s32, "code_002CAFD8", cmpLeaf38);

INCLUDE_ASM(const s32, "code_002CAFD8", cmpLeaf39);

INCLUDE_ASM(const s32, "code_002CAFD8", cmpDrawTriGSub);

INCLUDE_ASM(const s32, "code_002CAFD8", cmpDrawSetup3DView);

void cmpDrawSpriteFlag(u8 id, s32 flag, s32 ot)
{
    sdfGrOt_t *otp;
    void *packet;
    u64 *prim;
    void *tag;

    packet = EE_ADDR_PTR(sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(1, 1)));
    sdf2DMakePrimPacketHeader(packet, 0, 1, 0xE, 1);
    prim = sdf2DGetPrimPtr(packet);
    prim[0] = id != 0;
    if (flag == 0) {
        prim[1] = 0x4A;
    } else {
        prim[1] = 0x4B;
    }
    tag = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}

INCLUDE_ASM(const s32, "code_002CAFD8", cmpLeaf40);

void cmpDrawSprite(s32 id, s32 flag, s32 ot)
{
    sdfGrOt_t *otp;
    void *packet;
    u64 *prim;
    void *tag;

    packet = EE_ADDR_PTR(sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(1, 1)));
    sdf2DMakePrimPacketHeader(packet, 0, 1, 0xE, 1);
    prim = sdf2DGetPrimPtr(packet);
    prim[0] = id;
    if (flag == 0) {
        prim[1] = 0x47;
    } else {
        prim[1] = 0x48;
    }
    tag = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}

INCLUDE_ASM(const s32, "code_002CAFD8", fclDrawPanelSprite);

void cmpDrawSprite2(s32 id, s32 flag, s32 ot)
{
    sdfGrOt_t *otp;
    void *packet;
    u64 *prim;
    void *tag;

    packet = EE_ADDR_PTR(sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(1, 1)));
    sdf2DMakePrimPacketHeader(packet, 0, 1, 0xE, 1);
    prim = sdf2DGetPrimPtr(packet);
    prim[0] = id;
    if (flag == 0) {
        prim[1] = 0x42;
    } else {
        prim[1] = 0x43;
    }
    tag = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}

INCLUDE_ASM(const s32, "code_002CAFD8", fclDrawPanelSprite2);

void cmpDrawSpriteIndexed(s32 id, s32 ot)
{
    sdfGrOt_t *otp;
    void *packet;
    u64 *prim;
    void *tag;

    packet = EE_ADDR_PTR(sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(1, 1)));
    sdf2DMakePrimPacketHeader(packet, 0, 1, 0xE, 1);
    prim = sdf2DGetPrimPtr(packet);
    prim[1] = 0x49;
    prim[0] = id;
    tag = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}

void cmpDrawSpriteIndexed2(s32 id, s32 ot)
{
    sdfGrOt_t *otp;
    void *packet;
    u64 *prim;
    void *tag;

    packet = EE_ADDR_PTR(sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(1, 1)));
    sdf2DMakePrimPacketHeader(packet, 0, 1, 0xE, 1);
    prim = sdf2DGetPrimPtr(packet);
    prim[1] = 0x14;
    prim[0] = id;
    tag = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}

INCLUDE_ASM(const s32, "code_002CAFD8", cmpLeaf41);

void uiDrawUniformRgbRange(const u32 *xs, const u32 *ys, u32 z, u32 color, u32 ot)
{
    u32 colors[3];

    colors[2] = color;
    colors[1] = color;
    colors[0] = color;
    cmpDrawTriG(xs, ys, z, colors, ot);
}

void cmpDrawTriG(const u32 *xs, const u32 *ys, u32 z, const u32 *colors, u32 ot)
{
    void *packet;
    struct { u64 colorChannels[2]; u64 packedXY; u64 depth; } *prim;
    sdfDrawTag_t *tag;
    sdfGrOt_t *otp;

    packet = EE_ADDR_PTR(sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(6, 1)));
    sdf2DMakePrimPacketHeader(packet, 0x14B, 6, 0x515151, 1);
    prim = sdf2DGetPrimPtr(packet);
    cmpExpandColor(prim[0].colorChannels, colors[0]);
    prim[0].packedXY = (xs[0] + 0x7000) | ((u64)(ys[0] + 0x7900) << 32);
    prim[0].depth = z;
    cmpExpandColor(prim[1].colorChannels, colors[1]);
    prim[1].packedXY = (xs[1] + 0x7000) | ((u64)(ys[1] + 0x7900) << 32);
    prim[1].depth = z;
    cmpExpandColor(prim[2].colorChannels, colors[2]);
    prim[2].packedXY = (xs[2] + 0x7000) | ((u64)(ys[2] + 0x7900) << 32);
    prim[2].depth = z;
    tag = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(sizeof(sdfDrawTag_t)));
    sdfDrawTagInit(tag);
    sdfDrawTagLink(tag, EE_ADDR(packet));
    otp = &sdfGrOtTable[ot];
    otp->regFunc(otp, tag);
}

void cmpDrawBoxSolid(s32 a, s32 b, s32 c, s32 d, s32 e, s32 color, s32 g, s32 h)
{
    s32 colors[4];

    colors[3] = color;
    colors[1] = color;
    colors[2] = color;
    colors[0] = color;
    cmpDrawBoxG(a, b, c, d, e, colors, g, h);
}

INCLUDE_ASM(const s32, "code_002CAFD8", cmpDrawBoxF);

INCLUDE_ASM(const s32, "code_002CAFD8", cmpDrawBoxG);

INCLUDE_ASM(const s32, "code_002CAFD8", cmpDrawBoxGDefault);

INCLUDE_ASM(const s32, "code_002CAFD8", cmpCallDrawPanelEffCell);

void cmpDrawPanelEffCell(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h)
{
    u32 pair[2];

    pair[1] = g;
    pair[0] = g;
    cmpDrawBoxLine(a, b, c, d, e, f, pair, h);
}

void cmpDrawBoxLine(u32 x0, u32 y0, u32 z0, u32 x1, u32 y1, u32 z1,
                    const u32 *colors, s32 surfaceIndex)
{
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;
    void *packet;
    u64 *prim;

    packet = EE_ADDR_PTR(sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(4, 1)));
    sdf2DMakePrimPacketHeader(packet, 0x149, 4, 0x5151, 1);
    prim = sdf2DGetPrimPtr(packet);
    cmpExpandColor(prim, colors[0]);
    prim[2] = (x0 + 0x7000) | ((u64)(y0 + 0x7900) << 32);
    prim[3] = z0;
    cmpExpandColor(prim + 4, colors[1]);
    prim[6] = (x1 + 0x7000) | ((u64)(y1 + 0x7900) << 32);
    prim[7] = z1;
    list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(list);
    sdfDrawTagLink(list, EE_ADDR(packet));
    surface = &sdfGrOtTable[surfaceIndex];
    surface->regFunc(surface, list);
}

void cmpDraw3DPoly(const f32 *first, const f32 *second, const f32 *third,
                   const u32 *colors, s32 unused, sdfMat_t *asset, s32 surfaceIndex)
{
    sdf3DPrimPolyParm_t request;
    u32 vertexColors[3];
    f32 textureCoordinates[3][2];
    sdfVector4_t positions[3] __attribute__((aligned(16)));
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;
    u32 firstColor, secondColor, thirdColor;

    list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(sizeof(sdfDrawTag_t)));
    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    if (asset != NULL) {
        sdf3DPrimLinkMat(list, asset, 0);
    }
    memset(&request, 0, sizeof(request));
    firstColor = colors[0];
    secondColor = colors[1];
    thirdColor = colors[2];
    request.faceCount = 1;
    request.pointCount = 3;
    request.color = firstColor;
    request.vertex = positions;
    request.normal = NULL;
    request.uv = NULL;
    vertexColors[0] = firstColor;
    vertexColors[1] = secondColor;
    vertexColors[2] = thirdColor;
    request.vertexColor = vertexColors;
    SDF_VU0_COPYVECTOR(&positions[0], first);
    SDF_VU0_COPYVECTOR(&positions[1], second);
    SDF_VU0_COPYVECTOR(&positions[2], third);
    textureCoordinates[0][0] = 0.0f;
    textureCoordinates[0][1] = 0.0f;
    textureCoordinates[1][0] = 0.0f;
    textureCoordinates[1][1] = 0.0f;
    textureCoordinates[2][0] = 0.0f;
    textureCoordinates[2][1] = 0.0f;
    sdfDrawTagLink(list, EE_ADDR(sdf3DPrimMakePoly(&request)));
    surface = &sdfGrOtTable[surfaceIndex];
    surface->regFunc(surface, list);
}

INCLUDE_ASM(const s32, "code_002CAFD8", cmpCallGrAllocPacketBuff);

INCLUDE_ASM(const s32, "code_002CAFD8", cmpCallGrAllocPacketBuff2);

void uiDrawTexturedSurfaceAtFarDepth(s32 surface) {
    sdfSubmitGsTestOneRegisterPacket(0x30000, surface);
    uiDrawUniformColorRect(0, 0, 0xFFFFFF, 0x2000, 0xE00, 0, surface);
    sdfSubmitGsTestOneRegisterPacket(0x3000DL, surface);
    uiDrawActiveSurfaceRegion(surface);
}
