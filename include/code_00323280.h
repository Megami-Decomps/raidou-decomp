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

#endif
