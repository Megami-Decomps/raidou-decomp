#ifndef CMPFILE_H
#define CMPFILE_H

#include "common.h"
#include "enums.h"

struct sprHandle_t;
struct sdfMemHandle_t;
struct dds3ProcessID_t;

/* Work of the camp fade process: fade out, wait, fade in, then wait for the map to finish. */
typedef struct cmpFadeProcessData_t {
    u8 pad00[8];
    u16 flag;
    s8 step;
    u8 pad0B;
    s32 count;
} cmpFadeProcessData_t;

/* Camp sprite packs: the loaded file, its slot set, and the motion each one links (-1 for none). */
extern struct sdfMemHandle_t *cmpWork3[4];
extern struct sprHandle_t *evtWork6[4];
extern s32 evtData9[4];
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
s32 fldCreateLinkSub(CMP_SPR a_no);
s32 pipeCreateLink(CMP_SPR a_no);
s32 pipeCampCAMPPACKLB(void);
s32 fldCallGetProcessData3(struct dds3ProcessID_t *id);
void cmpCallSetOt2(s32 x, s32 y, s32 frame);
void cmpDrawBack(void);

#endif
