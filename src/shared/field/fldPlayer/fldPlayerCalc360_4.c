#include "shared/field/fldPlayer/decls.h"

f32 fldPlayerCalc360_4(f32 aKakudo) {
    f32 best;
    s32 index;
    f32 diff;
    while (aKakudo >= 360.0f) {
        aKakudo -= 360.0f;
    }
    while (aKakudo < 0.0f) {
        aKakudo += 360.0f;
    }
    best = 900.0f;
    index = -1;
    diff = fabsf(R360_LR(0.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 0;
    }
    diff = fabsf(R360_LR(90.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 2;
    }
    diff = fabsf(R360_LR(180.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 4;
    }
    diff = fabsf(R360_LR(270.0f, aKakudo));
    if (diff < best) {
        index = 6;
    }
    switch (index) {
    case 0:
        aKakudo = 0.0f;
        break;
    case 2:
        aKakudo = 90.0f;
        break;
    case 4:
        aKakudo = 180.0f;
        break;
    case 6:
        aKakudo = 270.0f;
        break;
    }
    return aKakudo;
}
