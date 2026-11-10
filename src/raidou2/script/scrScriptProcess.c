#include "common.h"

#include "shared/script/scrScriptProcess/scrLoadScriptFile.c"

#include "shared/script/scrScriptProcess/scrStartScriptProcess.c"

INCLUDE_ASM(const s32, "script/scrScriptProcess", dds3GlobalWorkClearFlags);

#include "shared/script/scrScriptProcess/scrStartScriptByName.c"

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_00114978);

#include "shared/script/scrScriptProcess/scrStartScript2.c"

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_001149C8);

#include "shared/script/scrScriptProcess/scrRunScriptEx.c"

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_00114A30);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_00114A48);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_00114AE0);

INCLUDE_ASM(const s32, "script/scrScriptProcess", func_00114B58);

#include "shared/script/scrScriptProcess/scrSearchScriptName.c"

#include "shared/script/scrScriptProcess/scrSearchProcedureName.c"

#include "shared/script/scrScriptProcess/scrSetUserData.c"

#include "shared/script/scrScriptProcess/scrGetUserData.c"

#include "shared/script/scrScriptProcess/scrScriptShutdown.c"

#include "shared/script/scrScriptProcess/scrRunScript.c"

#include "shared/script/scrScriptProcess/scrScriptProcess.c"
