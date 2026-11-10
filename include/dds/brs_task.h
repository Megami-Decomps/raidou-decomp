#ifndef DDS_BRS_TASK_H

#define DDS_BRS_TASK_H
#include "brs_task_poll_result.h"

/* COMPLETE is consumed into IDLE; only an ACTIVE lifecycle returns ACTIVE. */
BrsTaskPollResult brsTaskConsumeDone(void);

#endif /* DDS_BRS_TASK_H */
