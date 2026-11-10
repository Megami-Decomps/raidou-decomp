#include "shared/sdf/sdfSystem/decls.h"

void dds3InitSubsystems(u32 heapSize, u32 initialHeapSize, u32 packetBufferBytes) {
    HW_WRITE(T0_MODE, 0x83);
    HW_WRITE(T0_COUNT, 0);
    sdfChipInit(initialHeapSize);
    sdfMemInit(heapSize);
    sdfGsMemInit();
    sdfImageSetInit();
    sdfTexInit();
    sdfPadInitManager();
    sdfInitThreadSystem();
    sdfGrInit(packetBufferBytes);
    sdf3DInit(0x40);
    sdfCoreDrawInit(sdfCoreDrawBuffSize);
    sdfRandomizeByTime(NULL);
    sdfStartThreadAndWait(&sdfSystemThreadCB, sdfSystemThreadProc, sdfSystemThreadStack, 0x2000, 0x4C, 0);
}
