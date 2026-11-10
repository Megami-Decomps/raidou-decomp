#include "shared/sdf/sdfBillBoard/decls.h"

u32 sdfBillBoardVcGetPacketSize(s32 aNum) {
    return (aNum * 0x4c + 0x4bU) & 0xfffffff0;
}
