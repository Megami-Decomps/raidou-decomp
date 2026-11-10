#include "shared/file/fileManager/decls.h"

s32 pacFileCheckEnd(const fileRequest_t *pacFileHandle) {
    s32 end;

    if (pacFileHandle->kind == 1) {
        end = 0;
        if (pacFileHandle->pacrest != 0) {
            end = pacFileHandle->state == 6;
        }
        return end;
    }
    return pacFileHandle->state == 6;
}
