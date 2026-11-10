#include "shared/sdf/sdfDraw/decls.h"

void sdfPreDrawTagInit(sdfPreDrawTag_t *aTag) {
    aTag->next = 0;
    aTag->head = 0;
    aTag->tail = 0;
    aTag->pad = 0;
}
