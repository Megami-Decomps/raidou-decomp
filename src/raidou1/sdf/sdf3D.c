#include "common.h"

#include "shared/sdf/sdf3D/sdf3DMakeViewportPacket.c"

#include "shared/sdf/sdf3D/sdf3DSetCamera.c"

#include "shared/sdf/sdf3D/sdf3DSetDefaultLight.c"

#include "shared/sdf/sdf3D/sdf3DSetDefaultFog.c"

INCLUDE_ASM(const s32, "sdf/sdf3D", func_003686B0);

INCLUDE_ASM(const s32, "sdf/sdf3D", sdf3DMakeLightParallelPacket);

#include "shared/sdf/sdf3D/sdf3DMakeFogPacket.c"

#include "shared/sdf/sdf3D/sdf3DMakeDirectRefPacket.c"

#include "shared/sdf/sdf3D/sdf3DLinkInitPacket.c"

INCLUDE_ASM(const s32, "sdf/sdf3D", sdf3DInitSpecularTex2);

#include "shared/sdf/sdf3D/sdf3DInit.c"
INCLUDE_SDATA(const s32, "sdf/sdf3D", D_0046F30C);

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_0046F310);

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_0046F318);

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_0046F320);

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_0046F32C);

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_0046F330);

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_0046F338);

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_0046F339);

INCLUDE_SDATA(const s32, "sdf/sdf3D", D_0046F33A);

