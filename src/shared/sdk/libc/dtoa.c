#include "common.h"

INCLUDE_RODATA(const s32, "sdk/libc/dtoa", dtoaInfinityText);

INCLUDE_RODATA(const s32, "sdk/libc/dtoa", dtoaNanText);

INCLUDE_ASM(const s32, "sdk/libc/dtoa", _dtoa_r);

INCLUDE_RODATA(const s32, "sdk/libc/dtoa", dtoaLog10Slope);

INCLUDE_RODATA(const s32, "sdk/libc/dtoa", dtoaLog10Offset);

INCLUDE_RODATA(const s32, "sdk/libc/dtoa", dtoaLog10Of2);

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "sdk/libc/dtoa", jtbl_00468D70);
#else
INCLUDE_RODATA(const s32, "sdk/libc/dtoa", jtbl_004DB740);
#endif

INCLUDE_RODATA(const s32, "sdk/libc/dtoa", localeLeafTable);

INCLUDE_RODATA(const s32, "sdk/libc/dtoa", isnanCallLitodpTable);

const char localeEmptyStr[8] __attribute__((section(".rodata"))) = "";

INCLUDE_RODATA(const s32, "sdk/libc/dtoa", localeDecimalPoint);

const char localeCName[4] __attribute__((section(".rodata"))) = "C";

INCLUDE_RODATA(const s32, "sdk/libc/dtoa", _pow5multTable);

INCLUDE_RODATA(const s32, "sdk/libc/dtoa", _dtoa_rStr);

const f64 pow10Table[25] = { 1.0, 10.0, 100.0, 1000.0, 10000.0, 100000.0, 1000000.0, 10000000.0, 100000000.0, 1000000000.0, 10000000000.0, 100000000000.0, 1000000000000.0, 10000000000000.0, 100000000000000.0, 1000000000000000.0, 1e+16, 1e+17, 1e+18, 1e+19, 1e+20, 1e+21, 1e+22, 1e+23, 1e+24 };
#ifdef VERSION_RAIDOU1

INCLUDE_RODATA(const s32, "sdk/libc/dtoa", dtoaPow10Big);

INCLUDE_RODATA(const s32, "sdk/libc/dtoa", dtoaPow10Limit);
#endif
#ifdef VERSION_RAIDOU2

INCLUDE_RODATA(const s32, "sdk/libc/dtoa", dtoaPow10Big);

INCLUDE_RODATA(const s32, "sdk/libc/dtoa", dtoaPow10Limit);
#endif

