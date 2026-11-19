#ifndef CODE_003BF2E0_H
#define CODE_003BF2E0_H

#include "common.h"
#include "gohMain.h"
#include "libsdf.h"
#include "jmsMisc.h"
#include "itf.h"
#include "enums.h"

/* ee-g++ vtable slot: this adjustment, then the method. */
typedef struct jmsVtblEntry_t {
    s32 delta;
    void *func;
} jmsVtblEntry_t;

/* The object jmsFirstInitSub allocates (0xBC bytes); jmsLeaf12 and jmsRefDDS3GBWK construct it. */
typedef struct jmsWork_t {
    /* 0x00 */ jmsVtblEntry_t *vtbl;
    /* 0x04 */ sdfMemHandle_t *hFile;
    /* 0x08 */ u32 fileData;
    /* 0x0C */ sdfMemHandle_t *hFile2;
    /* 0x10 */ u32 fileData2;
    /* 0x14 */ jmsMemsetWork_t *gbwk;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C[0x90];
    /* 0xAC */ s32 unkAC;
    /* 0xB0 */ s32 unkB0;
    /* 0xB4 */ s32 unkB4;
    /* 0xB8 */ s32 unkB8;
} jmsWork_t;

/* HD MenuFlag: the event bit each jmsMenu entry needs, 0 for none. */
extern u16 SetRootMenuData[];
/* HD strJmsMenu: each jmsMenu entry's text. */
extern char *cmpData34[];
s32 CheckFlagTGS(s32 MenuID);
void SetFlagMenu(dds3ProcessID_t *PID, s32 MenuID, ITFMENU_CB_GF Flag, s8 Bool);
void SetRootMenu(dds3ProcessID_t *PID, s32 MenuID, itfMenuFunc_t func);
void SetMenu(dds3ProcessID_t *PID, s32 MenuID, s32 ParentMenuID, itfMenuFunc_t func);
void jmsStart(void);

extern jmsVtblEntry_t jmsData12[34];
extern jmsVtblEntry_t jmsData13[33];
extern jmsWork_t *jmsLeaf11Data;
void jmsLeaf12(jmsWork_t *wk);
s32 jmsRefDDS3GBWK(jmsWork_t *wk);
void cmpCallBitCheck10(jmsWork_t *wk);
void rstCallGetStockDevilNums(gohData_t *work);
extern s32 rstCallBitCheckWork;

#endif
