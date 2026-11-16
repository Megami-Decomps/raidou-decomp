#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "lmap.h"
#include "jmsMisc.h"
#include "TitleMain.h"
#include "fldProcess.h"
#include "sndAt3.h"
#include "mviManager.h"
#include "scrCommand.h"
#include "evtEventViewer.h"
#include "evtPolygonMovie.h"
#include "code_r1_002CAFD8.h"
#include "dds3PadManager.h"
#include "code_r1_001CC1D0.h"
#include "sdfMovie.h"
#include "dds3KernelDraw.h"
#include "rstResult.h"
#include "code_r1_002C7ED0.h"
#include "sdfSound.h"
#include "facility.h"
#include "event.h"
#include "dds3SceneBasic.h"
#include "scr.h"
#include "fldNme.h"
#include "itf.h"
#include "code_r1_002C8188.h"
#include "sdf_resource.h"
#include "sdfMemory.h"
#include "cmbFont.h"
#include "nmeMain.h"
#include "Quest.h"
#include "mselGlobal.h"
#include "nmeInput0.h"
#include "scrTraceCode.h"
#include "sdk_libc.h"

s32 titleStartSub(s32 page)
{
    return titleStartSubData[page][0];
}

s32 titleStartSub2(s32 page)
{
    return titleStartSubData[page][1];
}

INCLUDE_ASM(const s32, "code_003004B0", titleFunc2);
