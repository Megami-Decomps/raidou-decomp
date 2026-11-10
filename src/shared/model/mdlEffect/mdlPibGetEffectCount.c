#include "shared/model/mdlEffect/decls.h"

u16 mdlPibGetEffectCount(pibHeaderChunk_t *record) {
    return record->parameter.part.partIndex;
}
