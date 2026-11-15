#include "common.h"

extern void dds3AdminiCallSequence(s32 requestedMode, const void *requestData, u32 dataBytes, s32 markHistory);
void dds3AdminSubmitEmptyModeRequest(void);

void dds3AdminSubmitEmptyModeRequest(void) {
    dds3AdminiCallSequence(0, 0, 0, 0);
}
