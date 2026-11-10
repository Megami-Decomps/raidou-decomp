#include "shared/sdf/sdfBillBoard/decls.h"

u32 sdfMultiBillBoardGetPacketSize(s32 aNum) {
    return (aNum * 0x54 + 0x4bU) & 0xfffffff0;
}
