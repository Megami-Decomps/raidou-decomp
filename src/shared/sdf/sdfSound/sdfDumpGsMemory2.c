#include "shared/sdf/sdfSound/decls.h"

void sdfDumpGsMemory2(void) {
    char buf[8];
    sdfGsMemSlice_t *head;
    sdfGsMemSlice_t *node;
    sdfGsMemSlice_t *walk;
    char *name;
    sdfPrintf(sdfGsMemInfoTitle);
    head = sdfGsMemGetLinkTail();
    node = head;
    while (node != 0) {
        if ((u32)node->type < SDF_TEX_RESOURCE_KNOWN_MODE_COUNT) {
            name = sdfGsMemTypeNamesTail[node->type];
        } else {
            sdfPrintf(buf, sdfGsMemTypeFmt, node->type);
            name = buf;
        }
        sdfPrintf(sdfGsMemNodeFmt, node, node->prev, node->next, name, node->addr, node->size);
        walk = head;
        while (walk != node) {
            walk = walk->prev;
        }
        node = node->prev;
    }
}
