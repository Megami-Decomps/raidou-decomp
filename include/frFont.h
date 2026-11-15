#ifndef FRFONT_H
#define FRFONT_H

#include "common.h"
#include "font.h"

frFontHandle_t *MessageRequest(s32 X, s32 Y, u8 *pMsg, frFontHandle_t *pFRQ);
s32 frDestroyFontRequestAfter(frFontHandle_t *pFRQ);
u8 frDisableMode(u8 Flag);
u8 frEnableMode(s32 Flag);
u32 frGetFontHandleNum(void);
s32 frGetTextHandleNum(void);
void frSetDrawPosition(frFontHandle_t *pFRQ, u32 X, u32 Y);
void frSetExtDic(u8 *pExtFontData);
void frSetFontColor(frFontHandle_t *pFRQ, u32 Color);
void frSetFontColorNo(frFontHandle_t *pFRQ, u8 ColorNo);
void frSetFontSpace(frFontHandle_t *pFRQ, s32 Space);
void frSetPriority(frFontHandle_t *pFRQ, u32 Pri);
void frSetVariableRequest(s32 VarNo, s32 pStr);

#endif
