#include "shared/interface/itfTestMesManager/decls.h"

void TestMsgReloadTexture(void)
{
    if (testMsgTexture != NULL) {
        sdfTexFreeDelayed(testMsgTexture);
        testMsgTexture = NULL;
    }
    testMsgTexIndex = (testMsgTexIndex + 1) & 3;
    if (testMsgTexIndex != 3) {
        testMsgTexture = itfMiscLoadTexture(D_00326BC8[testMsgTexIndex]);
    }
}
