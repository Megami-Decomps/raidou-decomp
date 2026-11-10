#include "shared/model/mdlManager/decls.h"

void mdlPackDecode(s32 major, s32 minor, s32 regFlags, const u8 *buff, s32 leng)
{
    sdfPacDecodeCb_t state;

    sdfPacInit(&state, 0);
    if (regFlags & 2)
    {
        sdfPacSetStay(&state);
    }
    sdfPacDecode(&state, buff, leng);
    mdlPackSet(state.resrcHead, major, minor, regFlags);
    sdfPacFree(&state);
}
