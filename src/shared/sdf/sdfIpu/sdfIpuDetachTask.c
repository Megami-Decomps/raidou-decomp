#include "shared/sdf/sdfIpu/decls.h"

void sdfIpuDetachTask(sdfIpuDecodeCb_t *cb)
{
    sdfIpuDecodeCb_t **link = &sdfIpuTaskLink;
    sdfIpuDecodeCb_t *cur;

    for (cur = *link; cur != NULL; cur = *link) {
        if (cur == cb) {
            *link = cur->next;
            return;
        }
        link = &cur->next;
    }
}
