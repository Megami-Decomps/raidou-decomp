#ifndef DDS_ITF_MES_SELECT_DEF_KEY_RESULT_H

#define DDS_ITF_MES_SELECT_DEF_KEY_RESULT_H
#include "common.h"

#include "itf_mes_window_index.h"

/* Setting an existing option and appending a new option are both success. */
typedef s32 ItfMesSelectDefKeyResult;

enum {
    ITF_MES_SELECT_DEF_KEY_REJECTED = 0,
    ITF_MES_SELECT_DEF_KEY_SET = 1
};

#endif /* DDS_ITF_MES_SELECT_DEF_KEY_RESULT_H */
