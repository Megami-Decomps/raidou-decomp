#include "common.h"

#include "shared/sdf/sdf3D/sdf3DMakeViewportPacket.c"

#include "shared/sdf/sdf3D/sdf3DSetCamera.c"

#include "shared/sdf/sdf3D/sdf3DSetDefaultLight.c"

#include "shared/sdf/sdf3D/sdf3DSetDefaultFog.c"

INCLUDE_ASM(const s32, "sdf/sdf3D", func_003DE438);

INCLUDE_ASM(const s32, "sdf/sdf3D", sdf3DMakeLightParallelPacket);

#include "shared/sdf/sdf3D/sdf3DMakeFogPacket.c"

#include "shared/sdf/sdf3D/sdf3DMakeDirectRefPacket.c"

#include "shared/sdf/sdf3D/sdf3DLinkInitPacket.c"

INCLUDE_ASM(const s32, "sdf/sdf3D", sdf3DInitSpecularTex2);

#include "shared/sdf/sdf3D/sdf3DInit.c"
INCLUDE_SDATA(const s32, "sdf/sdf3D", D_004E4324);

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_004E4328);

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_004E4330);

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_004E4338);

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_004E4344);

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_004E4348);

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_004E4350);

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_004E4351);

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_004E4352);

