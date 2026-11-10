#include "shared/sdf/sdfTmx/decls.h"

void LoadMcModules(const char *prefix, s32 moduleGroup) {
    char path[0x100];
    char **moduleNames = &mcModuleNames[moduleGroup * SDF_IOP_MODULES_PER_GROUP];
    s32 moduleIndex;

    for (moduleIndex = 0; moduleIndex != SDF_IOP_MODULES_PER_GROUP; moduleIndex++) {
        strcpy(path, prefix);
        strcat(path, moduleNames[moduleIndex]);
        sceSifLoadModule(path, 0, 0);
    }
    sceMcInit();
}
