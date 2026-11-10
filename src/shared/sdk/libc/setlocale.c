#include "shared/sdk/libc/decls.h"

char *setlocale(s32 category, const char *locale) {
    return _setlocale_r(_impure_ptr, category, locale);
}
