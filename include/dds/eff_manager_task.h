#ifndef DDS_EFF_MANAGER_TASK_H

#define DDS_EFF_MANAGER_TASK_H
#include "kwln.h"

/* These updates run each scheduler pass; RETAIN keeps each callback installed. */
dds3TaskUpdateResult_t ManagerRunF(dds3ProcessID_t *task);

void effManagerInitializeSubsystems(dds3ProcessID_t *task);

#endif /* DDS_EFF_MANAGER_TASK_H */
