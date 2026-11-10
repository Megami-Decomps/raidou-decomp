#include "shared/pb/pbGame/decls.h"

s8 pbCheckPreviousRecord(void) {
    s8 ret = pbCheckPopRecord(record);
    return ret;
}
