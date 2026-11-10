#include "shared/sdf/sdfBillBoard/decls.h"

u32 sdfMultiBillBoardVcGetPacketSize(s32 aNum) {
    return (aNum * 0x6c + 0x4bU) & 0xfffffff0;
}
