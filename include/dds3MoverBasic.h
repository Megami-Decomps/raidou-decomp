#ifndef DDS3MOVERBASIC_H
#define DDS3MOVERBASIC_H

#include "common.h"
#include "basic.h"

dds3Basic_t *dds3AddMoverBasic(dds3Basic_t *pObjectBasic);
void dds3MoverBasicFreePath(dds3Basic_t *pMoverBasic);
void dds3MoverBasicMakePath(dds3Basic_t *pMoverBasic);
void dds3MoverBasicSetCallBackPrimaryControl(dds3Basic_t *mover, dds3BasicControlProc_t func);
void dds3MoverBasicSetPath(dds3Basic_t *pMoverBasic, dds3Basic_t *pPathBasic);

s32 dds3MoverBasicUpdate(dds3Basic_t *pBasic);

#endif
