#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "fld.h"
#include "fldProcess.h"
#include "mapData.h"
#include "ctn.h"
#include "neko.h"
#include "lmap.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "dds3ObjectBase.h"
#include "code_r1_00128378.h"
#include "evtUnitManager.h"
#include "sdfSound.h"
#include "dds3KernelDraw_0010A750.h"
#include "dds3UnitObjectBasic.h"
#include "dds3Randomize.h"
#include "fldFileResolver.h"
#include "fldDraw.h"
#include "EventBit.h"
#include "evtStage.h"
#include "dds3KernelMain.h"
#include "dds3KernelDraw.h"
#include "mapLbWork.h"
#include "fldDbg.h"
#include "scrTraceCode.h"
#include "include_asm.h"
#include "font.h"
#include "frFont.h"
#include "sdfChip.h"
#include "sdf_resource.h"
#include "cmbFont.h"
#include "code_00183AB8.h"
#include "sdfMemory.h"
#include "sdfTex.h"

void nbLoadDeadMoonTex(void)
{
    u32 resource;
    sdfMemHandle_t *fileAllocation = sdfDevLoadFileEx(nbStrSpriteBg00Tmx, &resource, 0);

    DeadMoonTmx = sdfTexSet(EE_ADDR_AS(TMX_HEADER_t *, resource));
    sdfMemFreeDirect(fileAllocation);
}

INCLUDE_RODATA(const s32, "code_00183AB8", nbStrSpriteBg00Tmx);

