#ifndef NEWDATA_H
#define NEWDATA_H

#include "common.h"
#include "libsdf.h"
#include "facility.h"

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
    /* 0x22 */ u8 unk22[0x123E];
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
    /* 0x1C82 */ u8 unk1C82;
    /* 0x1C83 */ u8 unk1C83;
    /* 0x1C84 */ f32 moonTimer;
    /* 0x1C88 */ u8 unk1C88;
    /* 0x1C89 */ u8 unk1C89;
    /* 0x1C8A */ u8 unk1C8A[0x6];
    /* 0x1C90 */ u32 unk1C90;
    /* 0x1C94 */ u32 unk1C94;
    /* 0x1C98 */ u32 unk1C98;
    /* 0x1C9C */ u32 unk1C9C;
    /* 0x1CA0 */ u8 unk1CA0[0x20];
    /* 0x1CC0 */ u32 unk1CC0;
    /* 0x1CC4 */ u32 unk1CC4;
    /* 0x1CC8 */ s32 maka;
    /* 0x1CCC */ u8 unk1CCC[0x8];
    /* 0x1CD4 */ u32 unk1CD4;
    /* 0x1CD8 */ u8 unk1CD8[0x4];
    /* 0x1CDC */ u8 unk1CDC;
    /* 0x1CDD */ u8 unk1CDD[0xB];
    /* 0x1CE8 */ u32 unk1CE8;
    /* 0x1CEC */ u32 unk1CEC;
    /* 0x1CF0 */ u32 unk1CF0;
    /* 0x1CF4 */ u32 unk1CF4;
    /* 0x1CF8 */ u32 unk1CF8;
    /* 0x1CFC */ u16 unk1CFC;
    /* 0x1CFE */ u16 unk1CFE;
    /* 0x1D00 */ u16 unk1D00;
    /* 0x1D02 */ u16 unk1D02;
    /* 0x1D04 */ u8 unk1D04[0x10];
    /* 0x1D14 */ u16 unk1D14;
    /* 0x1D16 */ u16 unk1D16;
    /* 0x1D18 */ u16 unk1D18;
    /* 0x1D1A */ u8 unk1D1A[0x72];
    /* 0x1D8C */ u32 unk1D8C;
    /* 0x1D90 */ u8 unk1D90[0xC];
    /* 0x1D9C */ u32 unk1D9C;
    /* 0x1DA0 */ u8 unk1DA0[0x46];
    /* 0x1DE6 */ u16 unk1DE6;
    /* 0x1DE8 */ u16 unk1DE8;
    /* 0x1DEA */ u8 unk1DEA[0x2];
    /* 0x1DEC */ u32 unk1DEC;
    /* 0x1DF0 */ u32 unk1DF0;
    /* 0x1DF4 */ u32 unk1DF4;
    /* 0x1DF8 */ u8 unk1DF8[0x40];
    /* 0x1E38 */ u16 unk1E38;
    /* 0x1E3A */ u8 unk1E3A[0x13E];
    /* 0x1F78 */ u32 unk1F78;
    /* 0x1F7C */ u8 unk1F7C[0x2];
    /* 0x1F7E */ u16 unk1F7E;
    /* 0x1F80 */ u8 unk1F80[0x10];
    /* 0x1F90 */ u32 unk1F90;
    /* 0x1F94 */ u32 unk1F94;
    /* 0x1F98 */ u32 unk1F98;
    /* 0x1F9C */ u32 unk1F9C;
    /* 0x1FA0 */ u8 unk1FA0[0x90];
    /* 0x2030 */ u32 unk2030;
    /* 0x2034 */ u8 unk2034[0xAF4];
    /* 0x2B28 */ u32 unk2B28;
    /* 0x2B2C */ u8 unk2B2C[0x2];
    /* 0x2B2E */ u16 unk2B2E;
    /* 0x2B30 */ u8 unk2B30[0x10A];
    /* 0x2C3A */ u16 unk2C3A;
    /* 0x2C3C */ u8 unk2C3C;
    /* 0x2C3D */ u8 unk2C3D[0x3];
    /* 0x2C40 */ u32 unk2C40;
    /* 0x2C44 */ u32 unk2C44;
    /* 0x2C48 */ u8 unk2C48[0x28];
    /* 0x2C70 */ u16 unk2C70;
    /* 0x2C72 */ u16 unk2C72;
    /* 0x2C74 */ u8 unk2C74[0x4];
    /* 0x2C78 */ u32 unk2C78;
    /* 0x2C7C */ u8 unk2C7C[0xA34];
    /* 0x36B0 */ u32 unk36B0;
    /* 0x36B4 */ u32 unk36B4;
    /* 0x36B8 */ u8 unk36B8[0x1208];
    /* 0x48C0 */ u16 unk48C0;
    /* 0x48C2 */ u8 unk48C2[0x1E];
    /* 0x48E0 */ u16 unk48E0;
    /* 0x48E2 */ u8 unk48E2[0xE];
    /* 0x48F0 */ u32 unk48F0;
    /* 0x48F4 */ u8 unk48F4[0x264C];
    /* 0x6F40 */ u16 unk6F40;
    /* 0x6F42 */ u8 unk6F42[0x1E];
    /* 0x6F60 */ u8 unk6F60;
    /* 0x6F61 */ u8 unk6F61[0xF];
    /* 0x6F70 */ u32 unk6F70;
    /* 0x6F74 */ u32 unk6F74;
    /* 0x6F78 */ u32 unk6F78;
    /* 0x6F7C */ u32 unk6F7C;
    /* 0x6F80 */ u32 unk6F80;
    /* 0x6F84 */ u32 unk6F84;
    /* 0x6F88 */ u32 unk6F88;
    /* 0x6F8C */ u32 unk6F8C;
    /* 0x6F90 */ u32 unk6F90;
    /* 0x6F94 */ u32 unk6F94;
    /* 0x6F98 */ u32 unk6F98;
    /* 0x6F9C */ u32 unk6F9C;
    /* 0x6FA0 */ u32 unk6FA0;
    /* 0x6FA4 */ u32 unk6FA4;
    /* 0x6FA8 */ u32 unk6FA8;
    /* 0x6FAC */ u32 unk6FAC;
    /* 0x6FB0 */ u32 unk6FB0;
    /* 0x6FB4 */ u32 unk6FB4;
    /* 0x6FB8 */ u32 unk6FB8;
    /* 0x6FBC */ u32 unk6FBC;
    /* 0x6FC0 */ u32 unk6FC0;
    /* 0x6FC4 */ u8 unk6FC4[0x4];
    /* 0x6FC8 */ u32 unk6FC8;
    /* 0x6FCC */ u32 unk6FCC;
    /* 0x6FD0 */ u8 unk6FD0[0x20];
    /* 0x6FF0 */ u32 unk6FF0;
    /* 0x6FF4 */ u8 unk6FF4[0x27C];
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
    /* 0x1F82 */ u8 unk1F82;
    /* 0x1F83 */ u8 unk1F83;
    /* 0x1F84 */ f32 moonTimer;
    /* 0x1F88 */ u8 unk1F88;
    /* 0x1F89 */ u8 unk1F89;
    /* 0x1F8A */ u8 unk1F8A;
    /* 0x1F8B */ u8 unk1F8B;
    /* 0x1F8C */ u8 unk1F8C;
    /* 0x1F8D */ u8 unk1F8D;
    /* 0x1F8E */ u8 unk1F8E[0x2];
    /* 0x1F90 */ u32 unk1F90;
    /* 0x1F94 */ u32 unk1F94;
    /* 0x1F98 */ u32 unk1F98;
    /* 0x1F9C */ u32 unk1F9C;
    /* 0x1FA0 */ u8 unk1FA0[0x20];
    /* 0x1FC0 */ u32 unk1FC0;
    /* 0x1FC4 */ u32 unk1FC4;
    /* 0x1FC8 */ s32 maka;
    /* 0x1FCC */ u8 unk1FCC[0x8];
    /* 0x1FD4 */ u32 unk1FD4;
    /* 0x1FD8 */ u8 unk1FD8[0x4];
    /* 0x1FDC */ u8 unk1FDC;
    /* 0x1FDD */ u8 unk1FDD[0xB];
    /* 0x1FE8 */ u32 unk1FE8;
    /* 0x1FEC */ u32 unk1FEC;
    /* 0x1FF0 */ u32 unk1FF0;
    /* 0x1FF4 */ u32 unk1FF4;
    /* 0x1FF8 */ u32 unk1FF8;
    /* 0x1FFC */ u16 unk1FFC;
    /* 0x1FFE */ u16 unk1FFE;
    /* 0x2000 */ u16 unk2000;
    /* 0x2002 */ u16 unk2002;
    /* 0x2004 */ u8 unk2004[0x10];
    /* 0x2014 */ u16 unk2014;
    /* 0x2016 */ u16 unk2016;
    /* 0x2018 */ u16 unk2018;
    /* 0x201A */ u16 unk201A;
    /* 0x201C */ u8 unk201C[0x16];
    /* 0x2032 */ u8 unk2032[0x2];
    /* 0x2034 */ u8 unk2034[0x16];
    /* 0x204A */ u16 unk204A;
    /* 0x204C */ u8 unk204C[0x16];
    /* 0x2062 */ u16 unk2062;
    /* 0x2064 */ u8 unk2064[0x68];
    /* 0x20CC */ u32 unk20CC;
    /* 0x20D0 */ u8 unk20D0[0x8];
    /* 0x20D8 */ u32 unk20D8;
    /* 0x20DC */ u8 unk20DC[0x72];
    /* 0x214E */ u16 unk214E;
    /* 0x2150 */ u16 unk2150;
    /* 0x2152 */ u8 unk2152[0x6];
    /* 0x2158 */ u32 unk2158;
    /* 0x215C */ u32 unk215C;
    /* 0x2160 */ u8 unk2160[0x40];
    /* 0x21A0 */ u16 unk21A0;
    /* 0x21A2 */ u8 unk21A2[0x56];
    /* 0x21F8 */ u16 unk21F8;
    /* 0x21FA */ u8 unk21FA[0x2E6];
    /* 0x24E0 */ u32 unk24E0;
    /* 0x24E4 */ u8 unk24E4[0x2];
    /* 0x24E6 */ u16 unk24E6;
    /* 0x24E8 */ u8 unk24E8[0x10];
    /* 0x24F8 */ u32 unk24F8;
    /* 0x24FC */ u32 unk24FC;
    /* 0x2500 */ u32 unk2500;
    /* 0x2504 */ u32 unk2504;
    /* 0x2508 */ u8 unk2508[0xD0];
    /* 0x25D8 */ u32 unk25D8;
    /* 0x25DC */ u8 unk25DC[0x74];
    /* 0x2650 */ u16 unk2650;
    /* 0x2652 */ u8 unk2652[0xEB6];
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
    /* 0x48F0 */ u32 unk48F0;
    /* 0x48F4 */ u32 unk48F4;
    /* 0x48F8 */ u32 unk48F8;
    /* 0x48FC */ u32 unk48FC;
    /* 0x4900 */ u8 unk4900[0x30];
    /* 0x4930 */ u16 unk4930;
    /* 0x4932 */ u16 unk4932;
    /* 0x4934 */ u8 unk4934[0x158C];
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
    /* 0xBF18 */ u8 unkBF18[0x1298];
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



/* Model-visual records have a 0x310-byte native stride. The upstream packet
 * helpers use the scalar prefix; the action helpers index 0x1C-byte
 * visual-part records starting at +0x32, up to index 11. */
typedef struct
{
    u8 unk00;
    s8 unk01;
    u8 unk02[0x16];
    u16 actframe;
    u8 unk1A[2];
} datUnitVisualPart_t;

typedef struct datUnitVisual_t
{
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
    void *motion;
    u16 unk30;
    datUnitVisualPart_t part[12];
    u8 unk182[0x18E];
} datUnitVisual_t;

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
