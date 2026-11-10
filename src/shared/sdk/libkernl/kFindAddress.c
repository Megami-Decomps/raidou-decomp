#include "shared/sdk/libkernl/decls.h"

u32 *kFindAddress(u32 *start, const u32 *end, u32 instruction)
{
    while (*start != instruction && start < end) {
        start++;
    }
    if (start >= end) {
        start = NULL;
    }
    return start;
}
