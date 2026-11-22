#ifndef FCLMENU_H
#define FCLMENU_H

#include "common.h"

struct itfMover_t;
struct itfSprList_t;

extern struct itfSprList_t DrawCursorData;

u8 CalcAlpha(struct itfMover_t *mv);
void DrawCursor(s32 a_x, s32 a_y, u8 a_alpha);
void DrawCursorSelect(s32 a_x, s32 a_y, u8 a_alpha);
void DrawFontAlpha(s32 a_x, s32 a_y, u8 a_alpha, s8 a_attr, char *a_str);
void DrawPanel(s32 a_x, s32 a_y, u8 a_alpha, s8 a_attr, char *a_str);
void DrawSpriteAlpha(s32 a_x, s32 a_y, s32 a_z, s32 a_alpha, s32 a_sprNo);
void DrawSpriteList(s32 a_x, s32 a_y, s32 a_z, s32 a_alpha, struct itfSprList_t *a_list);
void DrawTitle(s32 a_x, s32 a_y, u8 a_alpha, char *a_str);
void *GetMenu(s32 a_hierarchy, s32 a_no);
void *GetSelectMenu(void);
void Init_fclMenu(void);
void MoverSetApper(void);
void MoverSetDisapper(void);
s32 MoverUpdate(void);
void Select_fclMenu(void);
void UpdateNextLink(void);
void UpdatePrevLink(void);
void UpdateTargetMenu(void);
void fclCreate(s32 a_nMenu);
void fclDraw(void);
void fclLoadData(const char *name);
void ButtonLineHelpDispSet(void);
s32 fclGetHierarchy(void);
u8 fclGetPanelAlpha(void);
s16 fclGetSelectId(void);
s16 fclGetSelectIdEx(void);
u8 fclGetTitleAlpha(void);
u8 fclGetTitleAlphaRate(void);
void fclSetApperTitle(void);
void fclSetDisapperTitle(void);
s32 fclSetFlagDecideEnd(s16 a_id);
s32 fclSetFlagSelect(s16 a_id, s32 a_bSelect);
void fclSetLink(void);
s32 fclSetParam(s16 a_id, s16 a_parentId, s32 a_bSelect, s32 a_bDrawTitle, char *a_str);
s32 fclSetParamFull(s16 a_id, s16 a_type, s16 a_parentId, s32 a_bSelect, s32 a_bDrawTitle, s32 a_posx, s32 a_posy, char *a_str);
void fclSetParamTitle(char *a_str);
void fclSetTargetId(s16 a_id);
void fclStartUpdate(void);
s16 fclUpdate(void);

#endif
