#ifndef DDS_DDS3_PROCESS_REGISTRATION_H

#define DDS_DDS3_PROCESS_REGISTRATION_H
#include "common.h"

/* Outcome of searching the scheduler queues for a task handle. */
typedef s32 dds3ProcessRegistrationResult_t;

#define DDS3_PROCESS_NOT_REGISTERED 0

#define DDS3_PROCESS_REGISTERED 1

struct dds3ProcessID_t;

/* Test name membership in the scheduler's registered task queues. */
dds3ProcessRegistrationResult_t sdfTaskExists(const char *name);

#endif /* DDS_DDS3_PROCESS_REGISTRATION_H */
