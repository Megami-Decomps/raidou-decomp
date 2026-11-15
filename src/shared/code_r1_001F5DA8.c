#include "common.h"
#include "dds3AdminiProcess.h"

void dds3AdminSubmitEmptyModeRequest(void);

void dds3AdminSubmitEmptyModeRequest(void) {
    dds3AdminiCallSequence(0, 0, 0, 0);
}
