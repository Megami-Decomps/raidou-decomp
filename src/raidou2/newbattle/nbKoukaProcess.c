#include "common.h"
#include "newbattle.h"
#include "kernel.h"
#include "nb.h"
#include "event.h"
#include "ps2asm.h"
#include "fpu.h"
#include "sdfFrame.h"

/* nbKoukaPacket_t.type, as the kouka dispatchers switch on it. Names and
 * values from the IDB; the code corroborates them, with 39 and 40 calling
 * effPCPBossBUNSIN* and 41 and 42 calling effPCPBossONGY*. */
typedef enum NB_KOUKA_TYPE {
    SKILLEFF = 0,
    TARMOVE = 1,
    HOMEMOVE = 2,
    ROT = 3,
    HOMEROT = 4,
    UNITCOL = 5,
    UNITHOMECOL = 6,
    UNITA = 7,
    UNITHOMEA = 8,
    DMGMOT = 9,
    BADDMGMOT = 10,
    NB_KOUKA_TYPE_RETURN = 11,
    NB_KOUKA_TYPE_SUMMON = 12,
    SAKE = 13,
    SKILLCOLOFF = 14,
    HITEFF = 15,
    NUMEFF = 16,
    HPMP = 17,
    PRESS = 18,
    BADSTAT = 19,
    BADMOTKAIFUKU = 20,
    SKILLEFF2 = 21,
    MOJIEFF = 22,
    BADEFF = 23,
    BADEFFKAIFUKU = 24,
    SE = 25,
    SKILLEFF1 = 26,
    MOTION = 27,
    SE2 = 28,
    SELOCK = 29,
    HITSTOP = 30,
    NEWPRESS = 31,
    MOTIONSTOP = 32,
    MOTIONSTOPCLS = 33,
    ANALYZE = 34,
    JYOKYOHELP = 35,
    DEADPCM = 36,
    BADMOT = 37,
    FORMONOFF = 38,
    BOSSONGBUN = 39,
    BOSSONGBUN2 = 40,
    BOSSONGOUT = 41,
    BOSSONGOUT2 = 42,
    SKILLEFF3 = 43,
    FORMFLAGONOFF = 44,
    NB_KOUKA_TYPE_FLY = 45,
    DEADEVENT = 46,
    FADEOUT = 47,
    FADEIN = 48,
    SKILLNAME = 49,
    HOJO = 50,
    FLARECHARGE = 51,
    PADSHOCK = 52,
    EASYDMGMOT = 53,
    SKILLCOLON = 54,
    SKILLMOTION = 55,
    STKPNL_FOUT = 56,
    STKPNL_END = 57,
    HATUDOEFF_END = 58
} NB_KOUKA_TYPE;

typedef union
{
    s8 c[4];
    u8 uc[4];
    s16 s[2];
    u16 us[2];
    s32 i;
    f32 f;
    void *p;
} nbKoukaData_u;

typedef struct nbKoukaPacket_t
{
    s16 startframe;
    s16 framelength;
    s8 type;
    u8 flag;
    u16 time;
    s32 uniqueid;
    struct nbKoukaPacket_t *next;
    struct nbKoukaPacket_t *prev;
    nbFormation_t *form;
    datUnitWork_t *unit;
    struct nbParty_t *party;
    s32 partyno;
    struct effBTLCutHandle_t *cuteff;
    nbKoukaData_u data1;
    nbKoukaData_u data2;
    sdfVector4_t v;
} nbKoukaPacket_t;

#include "sdk_libc.h"

extern void DelKoukaPacket(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);
extern void nbDelKoukaPacket(nbKoukaPacket_t *p);
void DelKoukaPacket(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);
void nbDelKoukaPacket(nbKoukaPacket_t *p);

#include "basic.h"
#include "dds3obj.h"

void nbDelKoukaPacket(nbKoukaPacket_t *p)
{
    DelKoukaPacket(nbGetKoukaProcessData(), p);
}
