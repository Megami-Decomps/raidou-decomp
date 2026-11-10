#include "shared/event/evtCommand/decls.h"

s32 evtCommand_FIELD_DSP(void)
{
    s32 area;
    s32 room;
    s32 mapno;

    area = scrGetIntPara(0);
    room = scrGetIntPara(1);
    mapno = (area << 16) + room;
    if (dds3GetMapNo(dds3GetControlScene()) != mapno) {
        area = scrGetIntPara(0);
        room = scrGetIntPara(1);
        evtLoadArea(area, room);
    }
    return 1;
}
