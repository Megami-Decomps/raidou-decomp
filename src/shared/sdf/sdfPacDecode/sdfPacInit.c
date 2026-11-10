#include "shared/sdf/sdfPacDecode/decls.h"

void sdfPacInit(sdfPacDecodeCb_t *cb, sdfPacCallbackFunc callback)
{
    memset(cb, 0, sizeof(*cb));
    cb->currBlockNum = -1;
    if (callback != NULL) {
        cb->callback = callback;
    } else {
        cb->callback = sdfPacStdCallback;
    }
}
