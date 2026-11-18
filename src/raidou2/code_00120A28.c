#include "common.h"
#include "basic.h"
#include "sdfChip.h"

void dds3PathBasicFreeControl(dds3Basic_t *pControl) {
    sdfChipFreeDirect(pControl->workdata);
}
