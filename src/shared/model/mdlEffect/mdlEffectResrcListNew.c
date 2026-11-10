#include "shared/model/mdlEffect/decls.h"

sdfItemArray_t *mdlEffectResrcListNew(u32 initCapa) {
    return sdfItemArrayNew(initCapa, 0x10, 4);
}
