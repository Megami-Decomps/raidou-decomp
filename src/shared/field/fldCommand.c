#include "common.h"
#include "basic.h"
#include "model.h"
#include "newdata.h"
#include "scr.h"
#include "field.h"
#include "ps2asm.h"
#include "sdfFrame.h"
#include "evtCommand.h"

typedef struct
{
    void *resourceState;
    u32 roomNumber;
} fldMatterWork_t;

typedef struct
{
    s32 unk00;
    s32 value;
} fldDanteParam_t;

typedef struct fldUnit_t fldUnit_t;

typedef struct fldInputPanel_t fldInputPanel_t;

enum sl_slot_flag
{
    SL_SLOT_FLAG_CAN_READ_SAVE = 1,
    SL_SLOT_FLAG_DIRECTORY_READY = 2,
    SL_SLOT_FLAG_DELETE_BEFORE_WRITE = 8
};

#include "sdk_libc.h"

extern char fldHojiNameKey[];
extern char *strstr(const char *, const char *);

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
    s16 flg;
    u8 spdx;
    u8 spdy;
    s16 life;
    s16 alp;
} fldSeedWork_t;

s32 fldCommand_ACT_WIN_DSP3(void);
s32 fldCommand_PAD_ENABLE(void);
u32 fldCommand_ENCOUNT_ON(void);
u32 fldCommand_ENCOUNT_OFF(void);
s32 fldCommand_CAM_SEL_PHOKAN(void);
s32 fldCommand_SET_NPC_CAMERA(void);
s32 fldCommand_GetHojiID(char *pName);
s32 fldCommand_SET_CAMTBL(void);
s32 fldCommand_FLD_KANKYO(void);
s32 fldCommand_FLD_FILTER(void);
s32 fldCommand_FLD_MODEL_ON(void);
s32 fldCommand_FLD_ANIME_ON(void);
s32 fldCommand_FLD_ANIME_OFF(void);
s32 fldCommand_FLD_ATARI_ON(void);
s32 fldCommand_FLD_MODEL_ON2(void);
s32 fldCommand_FLD_MODEL_OFF2(void);
s32 fldCommand_FLD_ANIME_ON2(void);
s32 fldCommand_FLD_ANIME_OFF2(void);
s32 fldCommand_FLD_ANIME_ID2(void);
s32 fldCommand_FLD_MAP_ON(void);
s32 fldCommand_FLD_MAP_OFF(void);
s32 fldCommand_FLD_CHK_BTL(void);
s32 fldCommand_FLD_CHK_ANA(void);
s32 fldCommand_FLD_CHK_SISETU(void);
u32 fldCommand_FLD_SET_TAKARA_OPEN(void);
u32 fldCommand_FLD_CHK_TAKARA_OPEN(void);
u32 fldCommand_FLD_WARP_HASIGO_GETTBL(void);
u32 fldCommand_FLD_WARP_ELV_GETTBL(void);
u32 fldCommand_FLD_WARP_ELV_EXIT(void);
u32 fldCommand_FLD_ELV_FLOORSTART(void);
u32 fldCommand_FLD_ELV_FLOOREND(void);
u32 fldCommand_FLD_ELV_ENDCHK(void);
s32 fldCommand_FLD_WARP_GET_DOOR(void);
u32 fldCommand_FLD_WARP_RUN_START(void);
s32 fldCommand_FLD_INF_NEXT(void);
s32 fldCommand_FLD_INF_GET(void);
void fldCommand_FLD_INF_GETPANEL(void);
s32 fldCommand_FLD_INF_GOWAP(void);
s32 fldCommand_FLD_MSE_PLAY2(void);
u32 fldCommand_FLD_MSE_STOP2(void);
s32 fldCommand_FLD_SET_ENCOUNT(void);
s32 fldCommand_FLD_EXIT_CAMERA(void);
s32 fldCommand_FLD_WARP_GET_WARPEFF(void);

typedef struct {
    u16 item;
    u16 par;
} datTakaraSagasiItem_t;

typedef struct {
    u32 flag;
    datTakaraSagasiItem_t itemtbl[8];
} datTakaraSagasiTable_t;

typedef struct {
    u32 flag;
    datTakaraSagasiTable_t tbl[4];
} datTakaraSagasi_t;

s32 fldCommand_FLD_GET_KIRETA(void);
s32 fldCommand_FLD_MODEL_DANTE(void);

s32 fldCommand_GetHojiID(char *pName) {
    s32 index;
    if (strstr(pName, fldHojiNameKey) == 0) return 0;
    for (index = 0; index < 32; index++) {
        if (pName[index] == '\0') {
            if (index < 4) return 0;
            return (pName[index - 2] - '0') * 10 + (pName[index - 1] - '0');
        }
    }
    return 0;
}

char fldHojiNameKey[8] = "hoji";
