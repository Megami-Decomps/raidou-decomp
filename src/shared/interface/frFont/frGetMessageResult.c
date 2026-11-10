#include "shared/interface/frFont/decls.h"

u32 frGetMessageResult(u32 Flag) {
    return gMessageRequestResult & Flag;
}
