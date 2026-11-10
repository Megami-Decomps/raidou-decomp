#include "shared/sdf/sdfBillBoard/decls.h"

s32 sdfBillBoardGetPacketSize(s32 aNum) {
    return (aNum << 6) + 0x40;
}
