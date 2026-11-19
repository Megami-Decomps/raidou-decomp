#ifndef CMPFILE_H
#define CMPFILE_H

#include "common.h"
#include "enums.h"

struct sprHandle_t;
struct sdfMemHandle_t;
struct dds3ProcessID_t;

/* Work of the camp fade process: fade out, wait, fade in, then wait for the map to finish. */
typedef struct tagCMP_MAP_WORK {
    struct sdfMemHandle_t *handle;
    struct dds3ProcessID_t *drawPID;
    u16 flag;
    s8 step;
    u8 pad0B;
    s32 count;
} tagCMP_MAP_WORK;

/* Camp sprite packs: the loaded file, its slot set, and the motion each one links (-1 for none). */
#ifdef VERSION_RAIDOU1
extern struct sdfMemHandle_t *cmpWork3[4];
extern struct sprHandle_t *evtWork6[4];
extern s32 evtData9[4];
#else
extern struct sdfMemHandle_t *cmpWork3[CMP_SPR_NUM];
extern struct sprHandle_t *evtWork6[CMP_SPR_NUM];
extern s32 evtData9[CMP_SPR_NUM];
extern s32 cmpWork8[CMP_SPR_NUM]; /* load count of each pack */
extern s32 (*cmpResetMotFunc[2])(void);
#endif
extern s32 (*cmpLinkMotFunc[2])(void);
extern void *cmpFreeScriptData;
extern u32 effScalyTextureHandle2;
extern s32 cmpDrawBackData;
extern const char cmpStrCampScriptCmpUseScrBf[];
extern const char pipeStrCampCamppackLb[];

struct sprHandle_t *cmpGetSpriteHnd(CMP_SPR a_no);
struct sdfMemHandle_t *cmpLoadSpriteHnd(CMP_SPR a_no);
void cmpLoadScript(void);
u8 evtFunc18(s32 a_no);
s32 evtFunc19(CMP_SPR a_no);
s32 evtCreateLinkSub(CMP_SPR a_no);
s32 cmpFreeSpriteHnd(CMP_SPR a_no);
s32 pipeCreateLink(CMP_SPR a_no);
s32 pipeCampCAMPPACKLB(void);
s32 Process_cmpMap(struct dds3ProcessID_t *id);
void cmpCallSetOt2(s32 x, s32 y, s32 frame);
void cmpDrawBack(tagCMP_MAP_WORK *wk);
s32 cmpFreeSpriteMem(CMP_SPR a_no);
s32 cmpLoadSpriteMem(CMP_SPR a_no);

#endif
