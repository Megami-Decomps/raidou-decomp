#include "shared/interface/itfMesManager/decls.h"

s32 CalcMsgMng_Process(void) {
    itfMesLink_t *node = itfMesMngManager.pool.activeHead;
    while (node != 0) {
        CalcMesFlow(node->state);
        node = node->next;
    }
    return 0;
}
