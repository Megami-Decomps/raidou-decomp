#ifndef DDS_FLD_FILE_IO_PRIORITY_STATE_H

#define DDS_FLD_FILE_IO_PRIORITY_STATE_H
#include "common.h"

/* The main loop uses this state to select the file command thread's priority. */
typedef s8 FldFileIoPriorityState;

enum {
    FLD_FILE_IO_PRIORITY_HIGH = 0,
    FLD_FILE_IO_PRIORITY_LOW = 1
};

#endif /* DDS_FLD_FILE_IO_PRIORITY_STATE_H */
