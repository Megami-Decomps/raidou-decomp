#ifndef DDS_BTL_TASK_POLL_RESULT_H

#define DDS_BTL_TASK_POLL_RESULT_H
#include "common.h"

/* btlRunTask continues at zero and enters end-delay for any nonzero result. */
typedef s32 BtlTaskPollResult;

enum {
    BTL_TASK_POLL_CONTINUE = 0,
    BTL_TASK_POLL_END_DELAY = 1,
};

#endif /* DDS_BTL_TASK_POLL_RESULT_H */
