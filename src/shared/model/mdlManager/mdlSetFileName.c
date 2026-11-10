#include "shared/model/mdlManager/decls.h"

char *mdlSetFileName(char *buff, const char *fname) {
    *(mdlPathPrefix_t *)buff = *(mdlPathPrefix_t *)mdlModelPathPrefix;
    return strcat(buff, fname);
}
