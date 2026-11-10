#include "shared/sdk/libc/decls.h"

long strtol(const char *s, char **end, s32 base) {
    return _strtol_r(_impure_ptr, s, end, base);
}
