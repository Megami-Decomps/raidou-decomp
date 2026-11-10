#include "common.h"

INCLUDE_ASM(const s32, "sdk/libc/signal", _init_signal_r);

INCLUDE_ASM(const s32, "sdk/libc/signal", _signal_r);

INCLUDE_ASM(const s32, "sdk/libc/signal", _raise_r);

INCLUDE_ASM(const s32, "sdk/libc/signal", __sigtramp_r);

INCLUDE_ASM(const s32, "sdk/libc/signal", raise);

INCLUDE_ASM(const s32, "sdk/libc/signal", signal);

INCLUDE_ASM(const s32, "sdk/libc/signal", _init_signal);
