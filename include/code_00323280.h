#ifndef CODE_00323280_H
#define CODE_00323280_H

#include "common.h"
#include "kernel.h"
extern dds3ProcessID_t *g_mainPID_cmpMain;
extern dds3ProcessID_t *g_mainPID_cmpMap;
/* The map frame's sprite list, drawn first by DrawTop. */
extern struct itfSprList_t DrawTopData[];

struct tagCMP_MAP_WORK;
void DrawTop(struct tagCMP_MAP_WORK *wk);
void Release_cmpMap(struct tagCMP_MAP_WORK *wk);
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
    u32 unk08;
} fclJpcWork_t;
void fclCallExistProcess(void);
void evtCallMemFreeDirect(void);

#endif
