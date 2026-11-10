#include "shared/kernel/dds3SequenceList/decls.h"

void CallTitle(u32 flg, u32 *data)
{
    if (data == 0) {
        titleStart(0);
        return;
    }
    titleStart(*data);
}
