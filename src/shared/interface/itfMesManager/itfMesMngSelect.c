#include "shared/interface/itfMesManager/decls.h"

s32 itfMesMngSelect(void) {
    s32 window;
    s32 entry;
    instanceMes_tag *record;
    select_tag *options;

    window = scrGetMessageHandle();
    if (window < 0) {
        return 1;
    }
    record = pinst[window].mes;
    entry = scrGetIntPara(0);
    options = &record->select;
    if (options->phase == 0) {
        itfMesMngRequestSelect(window, entry);
    } else if (options->phase < 0) {
        options->phase = 0;
        scrSetIntReturnValue(options->select);
        itfMesMngRequestSelectRelease(window);
        return 1;
    }
    return 0;
}
