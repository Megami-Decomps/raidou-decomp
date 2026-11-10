#include "shared/file/fileManager/decls.h"

s32 pacCheckEnd(const fileRequest_t *pacHandle) {
    s32 end = 0;

    if (pacHandle->pacrest != 0) {
        end = pacHandle->state == 6;
    }
    return end;
}
