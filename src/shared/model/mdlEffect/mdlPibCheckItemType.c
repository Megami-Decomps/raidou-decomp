#include "shared/model/mdlEffect/decls.h"

s32 mdlPibCheckItemType(pibHeaderChunk_t *data, s32 type) {
    return data->group == type;
}
