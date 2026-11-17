#include "common.h"
#include "fld.h"
#include "frFont.h"
#include "mapData.h"

void mapCallEnableMode2(s32 x, s32 y, u32 color, s32 priority, const char *str) {
    frFontHandle_t *font;

    frEnableMode(1);
    font = frSetFontString((u8 *)str, 1, 0, 0, NULL);
    frSetFontSpace(font, -1);
    frSetDrawPosition(font, x * 16, y * 8);
    frSetFontColor(font, color);
    frDrawFontOt(font, 0, priority);
    frDestroyFontRequestAfter(font);
    frDisableMode(1);
}

void mapAmDrawBasyoName(s32 area, s32 floor) {
    s32 i;

    for (i = 0; i < 96; i++) {
        if (mapData2[i].fieldID == area && mapData2[i].floorID == floor) {
            mapCallEnableMode2(256 - (frCallGetFontWidth3(mapData2[i].name, 1, 1, -1) >> 1), 26, 0xA09DC380, 104,
                               mapData2[i].name);
            return;
        }
    }
}
