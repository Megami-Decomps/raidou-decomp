#ifndef CODE_R1_0019_D3_A8_H
#define CODE_R1_0019_D3_A8_H

#include "common.h"
#include "kernel.h"
#include "sdfTex.h"

#define SDF_GS_CONTEXT_1 0

typedef struct mdlDebugProcessEntry_t {
    const char *name;
    dds3ProcessFunc_t update;
} mdlDebugProcessEntry_t;

extern mdlDebugProcessEntry_t mdlData12[3];
extern sdfTexHandle_t *mapData153;
extern const char mdlStrModelViewer[];
extern const char mdlStrModelViewerEnd[];

extern s32 mdlInitializeViewerTasksAndResourcesData3;

void mapTitleEnd(void);
void mdlLoadViewerConfig(void);
void mdlMakeHddFileList(void);
void mdlRestartViewerPhaseTask(void);
s32 mdlViewer(dds3ProcessID_t *PID);
s32 mapTrainCheckStartPos(dds3ProcessID_t *PID);

extern const char mdlStr0202[];
extern const char mdlStr02022[];

#endif
