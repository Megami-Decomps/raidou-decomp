#include "shared/sdk/libc/decls.h"

char *_setlocale_r(_reent *r, s32 category, const char *locale)
{
    if (locale != NULL) {
        if (strcmp(locale, localeCName) != 0 && strcmp(locale, localeEmptyStr) != 0)
            return NULL;
        r->_current_category = category;
        r->_current_locale = locale;
    }
    return localeCName;
}
