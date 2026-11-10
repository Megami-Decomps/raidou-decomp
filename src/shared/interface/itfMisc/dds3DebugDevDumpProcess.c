#include "shared/interface/itfMisc/decls.h"

/* Retail "DbgDevDump" task callback: perform no dump work and return zero. */
s32 dds3DebugDevDumpProcess(dds3ProcessID_t *proc_id) {
    return 0;
}
