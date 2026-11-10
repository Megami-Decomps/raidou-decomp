#include "shared/file/fileManager/decls.h"

void fileSync(fileRequest_t *fileHandle) {
    while (pacFileCheckEnd(fileHandle) == 0) {
        sdfFileCmdThreadSetHiPrio();
        fileManager();
    }
}
