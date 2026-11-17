#ifndef CODE_0010F2C0_H
#define CODE_0010F2C0_H

#include "common.h"
#include "kernel.h"

struct itfGridTextListItem_t;
extern dds3ProcessID_t *sbtlDebugTimeGrphWork;
extern dds3ProcessID_t *nbData2;
extern char sbtlStrDebugTimeGrph[];
extern char nbStrUnitEdit[];
extern char sbtlData82[];           /* "0x%03X" */
extern char nbDebugNoStatusLabel[]; /* "-----" */
extern char nbData3[];                /* "" */
extern char sbtlLEVELData[], sbtlLEVELData2[], sbtlLEVELData3[], sbtlLEVELData4[], sbtlLEVELData5[];
extern char sbtlLEVELData6[], sbtlLEVELData7[], sbtlLEVELData8[], sbtlLEVELData9[], sbtlLEVELData10[];
extern char nbLEVELData[], nbLEVELData2[], nbLEVELData3[], nbLEVELData4[], nbLEVELData5[];
extern char nbLEVELData6[], nbLEVELData7[], nbLEVELData8[], nbLEVELData9[], nbLEVELData10[];
extern char nbTOTALData[];           /* "TOTAL" */
extern char GohInitSkillChangeData[]; /* "AGE" */
extern char nbNUMS2Data[];           /* "NUMS" */

/* UnitEdit debug process work (0x10 bytes, from nbCallMemAllocDirect). */
typedef struct nbUnitEditWork_t {
    struct sdfMemHandle_t *handle;      /* 0x00 */
    u32 unk04;
    s32 mode;                           /* 0x08: nbCallMemAllocDirect argument */
    struct itfGridTextWidget_t *widget; /* 0x0C */
} nbUnitEditWork_t;

/* One row of the UnitEdit menu: its label and the setter that builds the row's child list. */
typedef struct nbUnitEditRow_t {
    const char *label;
    void (*create)(struct itfGridTextListItem_t *item, u32 mode);
} nbUnitEditRow_t;

extern nbUnitEditRow_t nbCallMemAllocDirectData[7];

void sbtlLeaf45(dds3ProcessID_t *id);
void sbtlDebugTimeGrph(s8 enabled);
nbUnitEditWork_t *nbCallMemAllocDirect(s32 mode);
s32 nbGetEventForm(dds3ProcessID_t *id);
void nbCallGetProcessData31(dds3ProcessID_t *id);
s32 sbtlLeaf46(struct itfGridTextWidget_t *widget);
s32 sbtlBtlDrawParamSub(struct itfGridTextWidget_t *widget);
void sbtlCallGetDevilLevelUpUnit(struct itfGridTextWidget_t *widget, s32 mode);
void sbtlCallGetPlayerExp(struct itfGridTextWidget_t *widget, s32 mode);
void sbtlLeaf47(struct itfGridTextWidget_t *widget, struct itfGridTextListItem_t *item, char *buf, s32 size);
s32 sbtlCallGetStockDevilNums(struct itfGridTextWidget_t *widget);
s32 sbtlCallGetSmallFontFRQ2(s32 x, s32 y, s32 z, struct itfGridTextWidget_t *widget, s32 ot);
s32 sbtlCallGetSmallFontFRQ22(s32 x, s32 y, s32 z, struct itfGridTextWidget_t *widget, s32 ot);
s32 sbtlCallAddItem(struct itfGridTextWidget_t *widget);
s32 sbtlFunc8(struct itfGridTextWidget_t *list);
s32 nbRefDDS3GBWK9(struct itfGridTextWidget_t *widget);
s32 nbRefDDS3GBWK10(struct itfGridTextWidget_t *widget);
s32 nbCallGetSmallFontFRQ2(s32 x, s32 y, s32 z, struct itfGridTextWidget_t *widget, s32 ot);
void nbRefDDS3GBWK(struct itfGridTextWidget_t *widget, struct itfGridTextListItem_t *item, char *buf, s32 size);
s32 sbtlRefDDS3GBWK(struct itfGridTextWidget_t *widget);
s32 nbRefDDS3GBWK4(struct itfGridTextWidget_t *widget);
s32 nbRefDDS3GBWK5(struct itfGridTextWidget_t *widget);
s32 nbRefDDS3GBWK6(struct itfGridTextWidget_t *widget);
s32 nbRefDDS3GBWK7(struct itfGridTextWidget_t *widget);
s32 nbRefDDS3GBWK8(struct itfGridTextWidget_t *widget);
s32 sbtlRefDDS3GBWK16(struct itfGridTextWidget_t *widget);
s32 sbtlRefDDS3GBWK17(struct itfGridTextWidget_t *widget);
s32 sbtlRefDDS3GBWK18(struct itfGridTextWidget_t *widget);
s32 sbtlRefDDS3GBWK19(struct itfGridTextWidget_t *widget);
s32 sbtlRefDDS3GBWK7(struct itfGridTextWidget_t *widget);
s32 sbtlRefDDS3GBWK8(struct itfGridTextWidget_t *widget);
s32 sbtlRefDDS3GBWK9(struct itfGridTextWidget_t *widget);
s32 sbtlDrawParam(struct itfGridTextWidget_t *widget);
s32 sbtlFunc9(struct itfGridTextWidget_t *widget);
s32 sbtlRefDDS3GBWK10(struct itfGridTextWidget_t *widget);
s32 sbtlRefDDS3GBWK11(struct itfGridTextWidget_t *widget);
s32 sbtlRefDDS3GBWK12(struct itfGridTextWidget_t *widget);
s32 sbtlRefDDS3GBWK13(struct itfGridTextWidget_t *widget);
s32 sbtlRefDDS3GBWK14(struct itfGridTextWidget_t *widget);
s32 nbCallSetAgeOfMoon(struct itfGridTextWidget_t *widget);

#endif
