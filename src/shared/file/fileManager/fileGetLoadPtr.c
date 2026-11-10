#include "shared/file/fileManager/decls.h"

u8 *fileGetLoadPtr(const fileRequest_t *fileHandle) {
    return EE_ADDR_AS(u8 *, fileHandle->top);
}
