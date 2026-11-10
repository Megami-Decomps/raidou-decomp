#include "shared/sdk/libdma/decls.h"

sceDmaEnv *sceDmaGetEnv(sceDmaEnv *env)
{
    *env = sceDmaSavedEnv;
    return env;
}
