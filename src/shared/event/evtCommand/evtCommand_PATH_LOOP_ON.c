#include "shared/event/evtCommand/decls.h"

s32 evtCommand_PATH_LOOP_ON(void)
{
    s32 type;
    void *obj;

    type = 4;
    do {
        obj = evtSearchBasic(type, scrGetIntPara(0));
        type++;
    } while (type < 10 && obj == NULL);

    if (obj == NULL) {
        obj = fldGetMatter_From_ModelRangeID(scrGetIntPara(0));
        if (obj == NULL) {
            return 1;
        }
    }
    evtPathLoop(obj, 1);
    return 1;
}
