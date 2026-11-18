#ifndef CODE_00323280_H
#define CODE_00323280_H

#include "common.h"
#include "itf.h"
#include "kernel.h"

struct tagCMP_WORK;
extern dds3ProcessID_t *g_mainPID_cmpMain;
extern dds3ProcessID_t *g_mainPID_cmpMap;
/* The map frame's sprite list, drawn first by DrawTop. */
extern struct itfSprList_t DrawTopData[];

struct tagCMP_MAP_WORK;
void DrawTop(struct tagCMP_MAP_WORK *wk);
void Release_cmpMap(struct tagCMP_MAP_WORK *wk);
void cmpRelease(struct tagCMP_WORK *wk);
void cmpCallMenuSetMenuTitle(struct tagCMP_WORK *wk);
void cmpMenuMaskSet(struct tagCMP_WORK *wk);
s32 cmpCallMenuGetSelectID2(void);
void cmpDrawMenuFunc(s32 a_id, s32 a_x, s32 a_y, s32 a_z, u32 a_color, u8 a_alpha);
/* The subscreen's title and row texts, then its title, panel and cursor sprites. */
extern char *cmpCallMenuSetMenuTitleData[];
extern itfSprList_t cmpCallMenuSetMenuTitleData2[];
extern itfSprList_t cmpCallMenuSetMenuTitleData3[];
extern itfSprList_t cmpCallMenuSetMenuTitleData4[];
void Shutdown_cmpMap(dds3ProcessID_t *id);
extern dds3ProcessID_t *effBTLFieldColorProcess;
extern CMP_MODE cmpCallFuncData;

typedef struct shpDrawWork_t {
    u8 unk00[8];
    struct sprHandle_t *hSpr; /* 0x08 */
} shpDrawWork_t;

extern shpDrawWork_t *wk_shpDraw;
extern char *cmpDrawItemListHeaderData[];
void cmpFunc10(void);
void fclJpcMain(void);
s32 trmCallGetProcessData(dds3ProcessID_t *PID);
s32 trmCallGetProcessData2(dds3ProcessID_t *PID);
s32 fclCallGetProcessData2(dds3ProcessID_t *PID);
void evtClearDispatchVisualFlag(void);

/* Process data of the jpc_main process (fclCheckData). */
typedef struct fclJpcWork_t {
    struct sdfMemHandle_t *hMem;
    dds3ProcessID_t *drawPID;
    u8 step;  /* 0x08 */
    u8 flag;  /* 0x09: bit 0 ends the process */
    u8 unk0A[2];
} fclJpcWork_t;
void fclCallExistProcess(void);
void evtCallMemFreeDirect(void);
void cmpProcessStart_cmpMain(void);

#endif
