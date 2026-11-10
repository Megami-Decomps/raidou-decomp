#include "shared/kernel/dds3PadManager/decls.h"

s32 dds3PadVibrationTest(dds3ProcessID_t *task) {
    if (dds3GetProcessRunTime(task) % 0x4650 == 0) {
        dds3PadVibrationSet(0, 1, 0xF);
        dds3PadVibrationSet(1, (u8)(dds3GetRandIntA(0, 150) + 100), 30);
    }
    return 0;
}
