#include "common.h"
#include "sdfMemory.h"
#include "code_r1_0021CE60.h"

void texRelease(fileJobPayload_t *job) {
    struct sdfMemHandle_t *allocation = job->primary.allocation;

    if (allocation != NULL) {
        sdfMemFreeDirect(allocation);
        job->primary.offset = 0;
        job->primary.size = 0;
        job->primary.allocation = NULL;
    }
}
