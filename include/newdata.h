#ifndef NEWDATA_H
#define NEWDATA_H

#include "common.h"
#include "libsdf.h"
#include "facility.h"
#include "sdatUnit.h"


typedef struct datDevilFormat_s
{
    /* 0x00 */ u32 flag;
    /* 0x04 */ u8 race;
    /* 0x05 */ u8 level;
    /* 0x06 */ u16 hp;
    /* 0x08 */ u16 maxhp;
    /* 0x0A */ u16 mp;
    /* 0x0C */ u16 maxmp;
    /* 0x0E */ s16 aisyoid;
    /* 0x10 */ s8 param[6];
    /* 0x16 */ u16 skill[8];
    /* 0x26 */ s16 keisyotype;
    /* 0x28 */ u16 keisyoform;
    /* 0x2A */ u16 dropmakka;
    /* 0x2C */ u16 dropexp;
    /* 0x2E */ u8 dropitem[2];
    /* 0x30 */ u8 droppoint[2];
    /* 0x32 */ u16 specialbit;
    /* 0x34 */ u8 specialitem;
    /* 0x35 */ u8 specialpoint;
    /* 0x36 */ u8 hougyokupoint;
    /* 0x37 */ u8 masekipoint;
    /* 0x38 */ s8 attackattr;
    /* 0x39 */ u8 attackcnt;
    /* 0x3A */ u8 attackinterval;
    /* 0x3B */ u8 reserve1;
} datDevilFormat_t;

/* Field-flag rows and the persistent digging-party records used by the
 * upstream field helpers. Their strides are 0x1E and 0xEC respectively. */
typedef struct
{
    u16 modelFlag;
    u16 animeFlag;
    u16 atariFlag;
    u16 modelFlag2;
    u16 animeFlag2;
    u8 animeId2[0x10];
    u16 npcFlag;
    u16 hojiFlag;
} fldGbFlags_t;

typedef struct
{
    fldGbFlags_t flags[32];
} fldGbTypeBlock_t;

typedef struct
{
    s32 unk00;
    u32 flag;
    u16 devilId;
    u16 hp;
    union {
        struct {
            u8 unk0C[6];
            u16 badstatus;
            u32 exp;
        };
        struct {
            u8 statePrefix[4];
            u64 packedState;
        } __attribute__((packed));
    };
    u8 pad18[2];
    s8 level;
    u8 unk1B[0x21];
    u16 skillcnt;
    u16 skill[24];
    u8 unk6E[0x7E];
} fldAnahoriAkuma_t;

/* The IDB's field order at PS2 widths: narrowing its 0x120-byte record, where
 * skillcnt is s32 and skill is s32[24], gives this 0xEC record with no gap.
 * Three HD bodies pin the names against retail: rstChkAddSkill tests
 * skillcnt at +0x38, SetAction_AI stores nowcommand and nowindex at +0x6A and
 * +0x6C, and datGetBaseParam sums param, skillparam and mitamaparam.
 * See docs/idb-corrections.md. */
typedef struct datUnitWork_t
{
    /* 0x00 */ u32 flag;
    /* 0x04 */ u16 id;
    /* 0x06 */ u16 hp;
    /* 0x08 */ u16 maxhp;
    /* 0x0A */ u16 mp;
    /* 0x0C */ u16 maxmp;
    /* 0x0E */ u16 badstatus;
    /* 0x10 */ u32 exp;
    /* 0x14 */ u16 level;
    /* 0x16 */ s8 param[6];
    /* 0x1C */ s8 skillparam[6];
    /* 0x22 */ u16 uniqueid;
    /* 0x24 */ u16 reserve1;
    /* 0x26 */ u16 reserve2;
    /* 0x28 */ u16 namecode[8];
    /* 0x38 */ u16 skillcnt;
    /* 0x3A */ u16 skill[24];
    /* 0x6A */ s16 nowcommand;
    /* 0x6C */ u16 nowindex;
    /* 0x6E */ s16 nowtarea;
    /* 0x70 */ s16 nowtform;
    /* 0x72 */ s16 prevcommand;
    /* 0x74 */ u16 previndex;
    /* 0x76 */ s16 prevtarea;
    /* 0x78 */ s16 prevtform;
    /* 0x7A */ s8 mitamaparam[6];
    /* 0x80 */ s8 levelupparam[6];
    /* 0x86 */ u16 keisyoskill[24];
    /* 0xB6 */ u8 keiattr[24];
    /* 0xCE */ u16 hensinmae;
    /* 0xD0 */ u16 getdevilhearts[4];
    /* 0xD8 */ u32 hensinmaeexp;
    /* 0xDC */ u16 fullnamecode[8];
} datUnitWork_t;

typedef struct
{
    datUnitWork_t unit;
} cmbUnit_t;

/* One name character: a two-byte font code, read as a halfword or as its bytes. */
typedef union {
    u16 code;
    u8 bytes[2];
} dds3NameChar_t;

/* A 24x24 4-bit name glyph. */
typedef struct {
    u8 pixel[0x120];
} dds3NameImage_t;

/* Shared persistent work ("GBWK"), laid out per version from its consumers.
 * unkXXXX fields are accessed at that offset with that width; the rest is unknown. */
typedef struct
{
#ifdef VERSION_RAIDOU1
    /* 0x0 */ u8 ver[8];
    /* 0x8 */ s32 word08;
    /* 0xC */ s16 half0C;
    /* 0xE */ s16 transition;
    /* 0x10 */ u8 unk10[0x11];
    /* 0x21 */ u8 unk21;
    /* 0x22 */ u8 unk22[0xE];
    /* 0x30 */ u8 unk30[0x10];
    /* 0x40 */ u8 unk40[0x1220];
    /* 0x1260 */ u8 unk1260[0x2];
    /* 0x1262 */ u8 unk1262[0xE];
    /* 0x1270 */ sdfMemHandle_t *memHandle;
    /* 0x1274 */ u32 unk1274;
    /* 0x1278 */ u32 unk1278;
    /* 0x127C */ u8 unk127C[0x4];
    /* 0x1280 */ s32 integers[256];
    /* 0x1680 */ f32 floats[256];
    /* 0x1A80 */ u32 EventBit[128];
    /* 0x1C80 */ u8 moonFlags;
    /* 0x1C81 */ u8 moonPhase;
    /* 0x1C82 */ s8 takaraCnt;
    /* 0x1C83 */ s8 zeniCnt;
    /* 0x1C84 */ f32 moonTimer;
    /* 0x1C88 */ s8 takumiCnt;
    /* 0x1C89 */ s8 teisatuCnt;
    /* 0x1C8A */ u8 unk1C8A[0x6];
    /* 0x1C90 */ u32 configFlag;
    /* 0x1C94 */ u32 unk1C94;
    /* 0x1C98 */ u32 unk1C98;
    /* 0x1C9C */ s32 mapSummonID;
    /* 0x1CA0 */ SBTL_PLAYER_DATA playerData;
    /* 0x2C40 */ f32 encountkyori;
    /* 0x2C44 */ u32 encountcnt;
    /* 0x2C48 */ u8 unk2C48[0x28];
    /* 0x2C70 */ u16 unk2C70;
    /* 0x2C72 */ u16 unk2C72;
    /* 0x2C74 */ u8 unk2C74[0x4];
    /* 0x2C78 */ u32 unk2C78[14];
    /* 0x2CB0 */ u8 automapmask[80][32];
    /* 0x36B0 */ u32 unk36B0;
    /* 0x36B4 */ u32 unk36B4;
    /* 0x36B8 */ u8 unk36B8[0x8];
    /* 0x36C0 */ dds3NameImage_t nameImage[2][8];
    /* 0x48C0 */ dds3NameChar_t nameCode[2][8];
    /* 0x48E0 */ u8 nameLength[2];
    /* 0x48E2 */ u8 unk48E2[0xE];
    /* 0x48F0 */ u32 unk48F0;
    /* 0x48F4 */ u8 unk48F4[0x144C];
    /* 0x5D40 */ dds3NameImage_t nbNameImage[16];
    /* 0x6F40 */ dds3NameChar_t nbNameCode[16];
    /* 0x6F60 */ u8 unk6F60;
    /* 0x6F61 */ u8 unk6F61[0xF];
    /* 0x6F70 */ union { u32 word; s16 half; } pointTable[26];
    /* 0x6FD8 */ u8 unk6FD8[0x18];
    /* 0x6FF0 */ u32 unk6FF0;
    /* 0x6FF4 */ u8 unk6FF4[0x7C];
    /* 0x7070 */ u8 automapmask2[16][32];
#else
    /* 0x0 */ u8 ver[8];
    /* 0x8 */ s32 word08;
    /* 0xC */ s16 half0C;
    /* 0xE */ s16 transition;
    /* 0x10 */ u8 unk10[0x11];
    /* 0x21 */ u8 unk21;
    /* 0x22 */ u8 unk22;
    /* 0x23 */ u8 unk23;
    /* 0x24 */ u8 unk24;
    /* 0x25 */ u8 unk25;
    /* 0x26 */ u8 unk26[0x123A];
    /* 0x1260 */ u8 unk1260[0x2];
    /* 0x1262 */ u8 unk1262[0xE];
    /* 0x1270 */ sdfMemHandle_t *memHandle;
    /* 0x1274 */ u32 unk1274;
    /* 0x1278 */ u32 unk1278;
    /* 0x127C */ u32 unk127C;
    /* 0x1280 */ s32 integers[256];
    /* 0x1680 */ f32 floats[256];
    /* 0x1A80 */ u32 EventBit[256];
    /* 0x1E80 */ u8 unk1E80[0x100];
    /* 0x1F80 */ u8 moonFlags;
    /* 0x1F81 */ u8 moonPhase;
    /* 0x1F82 */ s8 takaraCnt;
    /* 0x1F83 */ s8 zeniCnt;
    /* 0x1F84 */ f32 moonTimer;
    /* 0x1F88 */ s8 takumiCnt;
    /* 0x1F89 */ s8 teisatuCnt;
    /* 0x1F8A */ u8 unk1F8A;
    /* 0x1F8B */ u8 unk1F8B;
    /* 0x1F8C */ u8 unk1F8C;
    /* 0x1F8D */ u8 unk1F8D;
    /* 0x1F8E */ u8 unk1F8E[0x2];
    /* 0x1F90 */ u32 configFlag;
    /* 0x1F94 */ u32 unk1F94;
    /* 0x1F98 */ u32 unk1F98;
    /* 0x1F9C */ s32 mapSummonID;
    /* 0x1FA0 */ SBTL_PLAYER_DATA playerData;
    /* 0x2F40 */ u8 unk2F40[0x5C8];
    /* 0x3508 */ u32 unk3508;
    /* 0x350C */ u8 unk350C[0x2];
    /* 0x350E */ u16 unk350E;
    /* 0x3510 */ u8 unk3510[0xD32];
    /* 0x4242 */ u16 unk4242;
    /* 0x4244 */ u16 unk4244;
    /* 0x4246 */ u8 unk4246;
    /* 0x4247 */ u8 unk4247[0x1];
    /* 0x4248 */ u16 unk4248;
    /* 0x424A */ u8 unk424A[0x1A];
    /* 0x4264 */ u8 unk4264;
    /* 0x4265 */ u8 unk4265;
    /* 0x4266 */ u8 unk4266;
    /* 0x4267 */ u8 unk4267[0x681];
    /* 0x48E8 */ u32 unk48E8;
    /* 0x48EC */ u16 unk48EC;
    /* 0x48EE */ u8 unk48EE[0x2];
    /* 0x48F0 */ f32 encountkyori;
    /* 0x48F4 */ u32 encountcnt;
    /* 0x48F8 */ u32 unk48F8;
    /* 0x48FC */ u32 unk48FC;
    /* 0x4900 */ u8 unk4900[0x30];
    /* 0x4930 */ u16 unk4930;
    /* 0x4932 */ u16 unk4932;
    /* 0x4934 */ u8 unk4934[0x4];
    /* 0x4938 */ u8 mesLog[0x1580]; /* message log, cleared by NewGameInit (HD MesLogData_t.mesLog) */
    /* 0x5EB8 */ u8 unk5EB8[0x8];
    /* 0x5EC0 */ u32 unk5EC0;
    /* 0x5EC4 */ u32 unk5EC4;
    /* 0x5EC8 */ u8 unk5EC8[0x1208];
    /* 0x70D0 */ u16 unk70D0;
    /* 0x70D2 */ u8 unk70D2[0x1E];
    /* 0x70F0 */ u16 unk70F0;
    /* 0x70F2 */ u8 unk70F2[0xE];
    /* 0x7100 */ u32 unk7100;
    /* 0x7104 */ u32 unk7104;
    /* 0x7108 */ u32 unk7108;
    /* 0x710C */ u8 unk710C[0x4];
    /* 0x7110 */ u32 unk7110;
    /* 0x7114 */ u8 unk7114[0x4E00];
    /* 0xBF14 */ u32 unkBF14;
    /* 0xBF18 */ u8 unkBF18[0x78];
    /* 0xBF90 */ dds3NameImage_t nbNameImage[16];
    /* 0xD190 */ dds3NameChar_t nbNameCode[16];
    /* 0xD1B0 */ u8 unkD1B0;
    /* 0xD1B1 */ u8 unkD1B1[0xF];
    /* 0xD1C0 */ u32 unkD1C0;
    /* 0xD1C4 */ u32 unkD1C4;
    /* 0xD1C8 */ u32 unkD1C8;
    /* 0xD1CC */ u32 unkD1CC;
    /* 0xD1D0 */ u32 unkD1D0;
    /* 0xD1D4 */ u32 unkD1D4;
    /* 0xD1D8 */ u32 unkD1D8;
    /* 0xD1DC */ u32 unkD1DC;
    /* 0xD1E0 */ u8 unkD1E0;
    /* 0xD1E1 */ u8 unkD1E1[0x3];
    /* 0xD1E4 */ u16 unkD1E4;
    /* 0xD1E6 */ u16 unkD1E6;
    /* 0xD1E8 */ u8 unkD1E8[0x198];
#endif
} dds3GlobalWork_t;
#ifdef VERSION_RAIDOU1
SIZE_CHECK(dds3GlobalWorkSizeCheck, dds3GlobalWork_t, 0x7270);
#else
SIZE_CHECK(dds3GlobalWorkSizeCheck, dds3GlobalWork_t, 0xD380);
#endif

extern dds3GlobalWork_t *DDS3_GBWK;
extern s32 btlConditionalEncounterCachedInterval;

typedef struct
{
    /* 0x00 */ s32 flag;
    /* 0x04 */ s32 flag2;
    /* 0x08 */ s32 encid;
    /* 0x0C */ s32 dbest;
} datBossPress_t;

typedef struct datDevilTableEntry_t
{
    /* 0x00 */ union { u32 flag; struct { u8 flagByte; u8 flagReserved[3]; }; };
    /* 0x04 */ u8 race;
    /* 0x05 */ u8 level;
    /* 0x06 */ u16 hp;
    /* 0x08 */ u16 maxhp;
    /* 0x0A */ u16 mp;
    /* 0x0C */ u16 maxmp;
    /* 0x0E */ s16 aisyoid;
    /* 0x10 */ s8 param[6];
    /* 0x16 */ u16 skill[8];
    /* 0x26 */ s16 keisyotype;
    /* 0x28 */ u16 keisyoform;
    /* 0x2A */ u16 dropmakka;
    /* 0x2C */ u16 dropexp;
    /* 0x2E */ u8 dropitem[2];
    /* 0x30 */ u8 droppoint[2];
    /* 0x32 */ u16 specialbit;
    /* 0x34 */ u8 specialitem;
    /* 0x35 */ u8 specialpoint;
    /* 0x36 */ u8 hougyokupoint;
    /* 0x37 */ u8 masekipoint;
    /* 0x38 */ s8 attackattr;
    /* 0x39 */ u8 attackcnt;
    /* 0x3A */ u8 attackinterval;
    /* 0x3B */ u8 reserve1;
} datDevilTableEntry_t;



/* The 0x1C-byte motion records at datUnitVisual_t +0x2C; the shared view splits
 * the same bytes as part[], six bytes later. */
typedef struct nbUnitMotion_t {
    s16 movetype;
    u16 actframe;
    union {
        s32 se_no;
        struct { u8 seBytes[2]; u8 type; s8 data; };
    };
    f32 motionsp;
    f32 atarisize;
    f32 movey;
    f32 movez;
    s16 bframe;
    s16 hframe;
} nbUnitMotion_t;


/* Complete motion-bank view of a native 0x310-byte model-visual row. */
typedef struct {
    sdfVector4_t center;
    f32 formsize;
    f32 formheight;
    f32 formscale;
    f32 oneshotr;
    f32 allshotr;
    u16 modelsize;
    u16 shadowsize;
    u16 badframe;
    u16 autowait;
    nbUnitMotion_t motions[26];
    u32 reserved[3];
} nbUnitVisualMotions_t;

SIZE_CHECK(nbUnitVisualMotions_size_must_be_0x310, nbUnitVisualMotions_t, 0x310);



/* One 0x14-byte motion entry of a unit visual row (HD datUnitMotion_t). */
typedef struct datUnitMotion_t
{
    s16 movetype;
    u16 actframe;
    struct datUnitMotEnd_t *end;
    f32 motionsp;
    f32 atarisize;
    struct datUnitMotHokan_t *hokan;
} datUnitMotion_t;

/* Raidou's 0x270-byte unit visual row (HD datUnitVisual_t with its motions inline). */
typedef struct datUnitVisual_t
{
    sdfVector4_t center;
    f32 formscale;
    f32 addz;
    f32 formsize;
    f32 formheight;
    f32 oneshotr;
    f32 allshotr;
    u16 shadowsize;
    u16 badframe;
    datUnitMotion_t motion[29];
} datUnitVisual_t;

SIZE_CHECK(datUnitVisual_size_must_be_0x270, datUnitVisual_t, 0x270);

/* Unit visual table read by evtCommandEFFBEDSTARTEVOSub and evtCallMemset9. */
extern datUnitVisual_t *evtData12;

typedef struct { u16 ritu, skill; u32 target; } datDevilAITbl_t;

typedef struct { u16 script, event; } datDevilAIDeath_t;

/* Full 0xA4-byte AI record; action tables begin at +0x28. */
typedef struct {
    u16 aiscript;
    u16 ailevel;
    struct { u32 joken[2]; s32 no; } entry[3];
    datDevilAITbl_t table[3][5];
    datDevilAIDeath_t death;
} datDevilAIData_t;

typedef struct { u8 ritu, item; } datNegoPresent_t;

typedef struct {
    s32 kucyo;
    u32 tokucyo;
    u8 tokuboutype, honboutype, fighttype, reqtype, tuikareqtype;
    u8 privatesel, privateselindex, senseitype, inotitype, binjotype;
    u16 mukantype, mukanindex;
    datNegoPresent_t itema[4], itemb[4], stone[4];
} datDevilNegoFormat_t;

extern datDevilNegoFormat_t master_text[];

typedef struct datSkill_t
{
    s8 skillattr;
    s8 type;
    s16 index;
    u16 keisyoform;
    s8 capacity;
    u8 flag;
} datSkill_t;

typedef struct
{
    u8 flag;
    u8 use;
    u8 koukatype;
    u8 costtype;
    u16 cost;
    u16 costbase;
    u8 targettype;
    u8 targetarea;
    u8 targetrule;
    u8 targetrandom;
    u16 untargetbadstat;
    u16 targetprog;
    u8 hittype;
    u8 hitlevel;
    u16 hitprog;
    u8 targetcntmin;
    u8 targetcntmax;
    u16 hptype;
    s16 hpn;
    u16 mptype;
    s16 mpn;
    s16 hpbase;
    s16 mpbase;
    u16 minus;
    u8 badtype;
    u8 badlevel;
    u16 basstatus;
    u32 hojotype;
    s8 hojopoint;
    u8 deadtype;
    u32 program;
    s16 criticalpoint;
    s16 failpoint;
    s16 magicbase;
    s16 magiclimit;
} datNormalSkill_t;

typedef struct
{
    u8 motion;
    u8 animetype;
    u8 hatudo;
    u8 bedno;
} datNormalSkillVisual_t;

typedef struct
{
    s16 n;
    s16 m;
    s16 a;
    s16 b;
} datSpecialSkill_t;

typedef struct
{
    datNormalSkill_t normal[288];
    u8 reserve[8];
    datNormalSkillVisual_t visual[288];
    datSpecialSkill_t special[95];
} datSkillTable_t;

extern datSkillTable_t datSkillTable;

typedef struct nbNegoSkillCondition_t {
    s32 ptype;
    s32 etype;
    s32 message;
} nbNegoSkillCondition_t;

typedef struct nbNegoSkillEntry_t {
    s32 requestmode;
    u8 rate[4];
    nbNegoSkillCondition_t condition[8];
} nbNegoSkillEntry_t;




/* Target codes for the AI's group and single target helpers. SMT3HD recovers this
 * enum in its own newdata.h and never cites it from a body, so the names come from
 * the definition alone; SetGroupTarget2 switches on all eleven values in order. */
typedef enum TAR_CODE {
    TAR_CODE_AI = 0,
    TAR_CODE_LIFEMIN = 1,
    TAR_CODE_BAD = 2,
    TAR_CODE_ID = 3,
    TAR_CODE_LIGHT = 4,
    TAR_CODE_DARK = 5,
    TAR_CODE_PLAYER = 6,
    TAR_CODE_JOKEN = 7,
    TAR_CODE_RND = 8,
    TAR_CODE_BAARLV = 9,
    TAR_CODE_LVMIN = 10,
} TAR_CODE;

#endif /* NEWDATA_H */
