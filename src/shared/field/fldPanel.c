#include "common.h"
#include "fldPanel.h"
#include "dds3KernelCore.h"
#include "sbtlWork.h"

s32 sbtlOnDataLoadInit(void) {
    dds3StartProcess("sbtleffonmem_load", 0x3F7, 1, 1, OnDataLoadProc, (dds3ProcessFunc_t)OnDataLoadShutdown, NULL);
    btlLoadSbPackData2 = 0;
    return 1;
}
