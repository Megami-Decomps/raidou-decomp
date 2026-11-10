#include "shared/sdk/libc/decls.h"

s32 atoi(const char *s) {
    return strtol(s, NULL, 10);
}
