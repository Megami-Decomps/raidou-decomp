#include "shared/sdf/sdfIpu/decls.h"

void sdfIpuAttachTask(sdfIpuDecodeCb_t *cb)
{
    sdfIpuDecodeCb_t **link = &sdfIpuTaskLink;
    sdfIpuDecodeCb_t *cur;

    for (cur = *link; cur != NULL; cur = *link) {
        link = &cur->next;
    }
    *link = cb;
    cb->next = NULL;
}
