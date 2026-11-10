#include "shared/interface/frFont/decls.h"

frFontHandle_t *MessageRequestEx2(s32 X, s32 Y, s32 Z, s32 Style, s32 ColNo, s32 Tp, s32 Spd, u8 *pMsg, frFontHandle_t *pFRQ) {
    frMsgInfo_t args;

    args.X = X;
    args.Y = Y;
    args.Z = Z << 4;
    args.Style = Style;
    args.ColNo = ColNo;
    args.Tp = Tp;
    args.Spd = Spd;
    args.pMsg = pMsg;
    args.pFRQ = pFRQ;
    args.Ofs = 0;
    args.FrqFlag = 1;
    args.RetFlag = 1;
    return frAnalyzeMessage(&args);
}
