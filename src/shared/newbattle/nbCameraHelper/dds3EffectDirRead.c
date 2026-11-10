#include "shared/newbattle/nbCameraHelper/decls.h"

/* Debug mode delegates to the native iterator. Built-in mode copies the next name,
   clears flag mask 0x1000, and returns name length; zero also marks table exhaustion. */
s32 dds3EffectDirRead(s32 directory, dds3EffectDirEnt_t *entry) {
    if (sdfHddValid != 0) {
        return sceDread(directory, entry);
    }
    if ((u32)dds3EffectBuiltinNameIndex >= EFF_BUILTIN_NAME_COUNT) {
        return 0;
    }
    strcpy(entry->name, dds3EffectFileNameTable[dds3EffectBuiltinNameIndex]);
    entry->flags &= ~EFF_DIRECTORY_FLAG_CLEAR;
    dds3EffectBuiltinNameIndex++;
    return strlen(entry->name);
}
