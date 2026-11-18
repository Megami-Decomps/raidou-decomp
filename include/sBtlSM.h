#ifndef SBTLSM_H
#define SBTLSM_H

#include "common.h"
#include "enums.h"
#include "sdfMemory.h"

struct SBTLUNIT_WORK;

/* Battle system message (0xBC bytes in R1). */
typedef struct SBTLSM_WORK {
    sdfMemHandle_t *handle;
    s32 gstat; /* SBTLSM_ST */
    SBTLSM mode;
    s32 counter;
    s32 cmpcounter;
    s32 listID;
    s32 devilid;
    s32 sklid;
    s32 raceid;
    u8 side;
    s32 ofs_x;
    s32 ofs_y;
    s32 alpha;
    char fontbuff[0x40];
    char fontbuff2[0x40];
    struct SBTLSM_WORK *prev;
    struct SBTLSM_WORK *next;
} SBTLSM_WORK;
SIZE_CHECK(sbtlSmWorkSizeCheck, SBTLSM_WORK, 0xBC);

typedef struct SBTLSM_LISTWORK {
    s32 num;
#ifdef VERSION_RAIDOU2
    s8 bLock; /* HD SBTLSM_LISTWORK.bLock; a nonzero lock holds the message counter */
    u8 pad05[3];
#endif
    SBTLSM_WORK *dwk;
} SBTLSM_LISTWORK;

#ifdef VERSION_RAIDOU2
#define SBTL_SKILL_NAME_SIZE 29
#define SBTL_LEAF74_NAME_SIZE 25
#else
#define SBTL_SKILL_NAME_SIZE 21
#define SBTL_LEAF74_NAME_SIZE 21
#endif

/* Name tables: race names (17 bytes), devil names (21 bytes), skill names. */
extern char (*sbtlData94)[17];
extern char (*g_itemNo)[21];
extern char (*sbtlData95)[SBTL_SKILL_NAME_SIZE];
extern char (*cmpSetupData4)[SBTL_LEAF74_NAME_SIZE];
extern char (*sbtlGetAisyoHelpFontData)[9];
#ifdef VERSION_RAIDOU2
extern char (*nbData24)[17];
#endif

s32 sbtlCallDeleteMessage(SBTLSM_LISTWORK *lwk);
s32 sbtlCallSetVarSBtlSM(SBTLSM_LISTWORK *lwk);
SBTLSM_LISTWORK *sbtlCreateSMList(void);
s32 sbtlFunc20(s32 a_x, s32 a_y, SBTLSM_WORK *dwk);
s32 sbtlAddMessage(SBTLSM_LISTWORK *lwk, SBTLSM_WORK *dwk, SBTLSM mode);
SBTLSM_WORK *sbtlCreateSM(SBTLSM_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, SBTLSM mode, SKLID sklid);
void DeleteMessage(SBTLSM_LISTWORK *lwk, SBTLSM_WORK *dwk);
void SetVar_sbtlSM(SBTLSM_WORK *dwk);
s32 sbtlDeleteSMList(SBTLSM_LISTWORK *lwk);
s32 sbtlReleaseSM_List(SBTLSM_LISTWORK *lwk, u8 flg);

#endif
