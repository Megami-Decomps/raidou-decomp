#include "shared/interface/itfMesManager/decls.h"

/* Return the same signed word forwarded as the script result when an option completes. */
s16 itfMesMngGetSelectNo(s32 msgid) {
    return pinst[msgid].mes->select.select;
}
