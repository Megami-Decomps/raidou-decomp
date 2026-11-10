#include "shared/script/scrScriptProcess/decls.h"

scrProcessWork_s *scrSearchScriptName(char *scriptname)
{
    scrProcessWork_s *pw;

    for (pw = scrHandleFirst; pw != NULL; pw = pw->next) {
        if (strcmp(scriptname, (char *)pw->Name) == 0) {
            return pw;
        }
    }
    return NULL;
}
