#include "common.h"

#include "shared/script/scrScriptProcess/scrLoadScriptFile.c"

#include "shared/script/scrScriptProcess/scrStartScriptProcess.c"

INCLUDE_ASM(const s32, "script/scrScriptProcess", dds3GlobalWorkClearFlags);

#include "shared/script/scrScriptProcess/scrStartScriptByName.c"

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_00112520);

#include "shared/script/scrScriptProcess/scrStartScript2.c"

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_00112570);

#include "shared/script/scrScriptProcess/scrRunScriptEx.c"

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_001125D8);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_001125F0);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_00112688);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_00112700);

#include "shared/script/scrScriptProcess/scrSearchScriptName.c"

#include "shared/script/scrScriptProcess/scrSearchProcedureName.c"

#include "shared/script/scrScriptProcess/scrSetUserData.c"

#include "shared/script/scrScriptProcess/scrGetUserData.c"

#include "shared/script/scrScriptProcess/scrScriptShutdown.c"

#include "shared/script/scrScriptProcess/scrRunScript.c"

#include "shared/script/scrScriptProcess/scrScriptProcess.c"

INCLUDE_RODATA(const s32, "script/scrScriptProcess", D_00444D88);

