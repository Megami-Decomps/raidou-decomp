#ifndef SBTLSCENE_H
#define SBTLSCENE_H

#include "common.h"
#include "sdf_types.h"

s32 BasicSceneFree(void);
s32 Init_sbtlScene(void);
s32 SetTestCamera(void);
s32 sbtlDefaultCutScroll(void);
void sbtlSetCutScroll(sdfVector4_t *pos);
s32 sbtlReLoadMap(void);
s32 sbtlSceneLoad(void);

#endif
