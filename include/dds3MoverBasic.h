#ifndef DDS3MOVERBASIC_H
#define DDS3MOVERBASIC_H

#include "common.h"
#include "basic.h"

typedef struct dds3MoverBasicWorkData_t
{
    dds3Basic_t *pObjectBasic;
    dds3PathBasicControl_t *pPathControl;
    dds3BasicControlProc_t pFunc;
    dds3Basic_t *pPathBasic;
} dds3MoverBasicWorkData_t;

dds3Basic_t *dds3AddMoverBasic(dds3Basic_t *pObjectBasic);
void dds3MoverBasicFreePath(dds3Basic_t *pMoverBasic);
void dds3MoverBasicMakePath(dds3Basic_t *pMoverBasic);
void dds3MoverBasicSetCallBackPrimaryControl(dds3Basic_t *mover, dds3BasicControlProc_t func);
void dds3MoverBasicSetPath(dds3Basic_t *pMoverBasic, dds3Basic_t *pPathBasic);
dds3PathBasicControl_t *dds3MoverBasicGetPathControl(dds3Basic_t *pMoverBasic);

s32 dds3MoverBasicUpdate(dds3Basic_t *pBasic);
s32 MoverBasicMainprocuder(dds3Basic_t *pBasic);

#endif
