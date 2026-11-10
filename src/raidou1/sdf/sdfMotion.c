#include "common.h"

#include "shared/sdf/sdfMotion/sdfMotionNodeNew.c"

#include "shared/sdf/sdfMotion/sdfMotionNodeFree.c"

#include "shared/sdf/sdfMotion/sdfMotionNodeBaseInit.c"

#include "shared/sdf/sdfMotion/sdfMotionCtrlrNew.c"

#include "shared/sdf/sdfMotion/sdfMotionCtrlrFree.c"

#include "shared/sdf/sdfMotion/sdfMotionChangeBlend.c"

#include "shared/sdf/sdfMotion/sdfMotionChange.c"

#include "shared/sdf/sdfMotion/sdfMotionSeek.c"

INCLUDE_ASM(const s32, "sdf/sdfMotion", sdfMotionNext);

#include "shared/sdf/sdfMotion/sdfMotionPause.c"

#include "shared/sdf/sdfMotion/sdfMotionPauseOff.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_Destroy.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_SetCut.c"

#include "shared/sdf/sdfMotion/sdfMotGetKey.c"

#include "shared/sdf/sdfMotion/sdfMotInterpolateKeyFloat.c"

#include "shared/sdf/sdfMotion/sdfMotInterpolateKeyXYZ.c"

#include "shared/sdf/sdfMotion/sdfMotInterpolateKeyColor.c"

#include "shared/sdf/sdfMotion/sdfMotInterpolateKeyUvwSub.c"

#include "shared/sdf/sdfMotion/sdfMotInterpolateKeyUvw.c"

#include "shared/sdf/sdfMotion/sdfMotGenNode.c"

#include "shared/sdf/sdfMotion/sdfMotGenModelInit.c"

#include "shared/sdf/sdfMotion/sdfMotGenModelLinerPos.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_modelLinerPosSeek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_modelLinerPosBlendSeek.c"

#include "shared/sdf/sdfMotion/sdfMotGenModelLinerRotXYZ.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_modelLinerRotXYZSeek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_modelLinerRotXYZBlendSeek.c"

#include "shared/sdf/sdfMotion/sdfMotGenModelLinerScale.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_modelLinerScaleSeek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_modelLinerScaleBlendSeek.c"

#include "shared/sdf/sdfMotion/sdfMotGenModelLerpRotQuat.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_modelLerpRotQuatSeek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_modelLerpRotQuatBlendSeek.c"

#include "shared/sdf/sdfMotion/sdfMotGenModelVisible.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_modelVisibleSeek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_modelVisibleBlendSeek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_modelVisibleInitBlend.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_modelPosInitBlend.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_modelRotInitBlend.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_modelScaleInitBlend.c"

#include "shared/sdf/sdfMotion/sdfMotGenMatNode.c"

#include "shared/sdf/sdfMotion/sdfMotGenMatInit.c"

#include "shared/sdf/sdfMotion/sdfMotGenMatDiffuse.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matDiffuseSeek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matDiffuseBlendSeek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matDiffuseInitBlend.c"

#include "shared/sdf/sdfMotion/sdfMotGenMatAmbient.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matAmbientSeek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matAmbientBlendSeek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matAmbientInitBlend.c"

#include "shared/sdf/sdfMotion/sdfMotGenMatSpecular.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matSpecularSeek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matSpecularBlendSeek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matSpecularInitBlend.c"

#include "shared/sdf/sdfMotion/sdfMotGenMatRefMap.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matRefMapSeek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matRefMapBlendSeek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matRefMapInitBlend.c"

#include "shared/sdf/sdfMotion/sdfMotGenMatConstant.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matConstantSeek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matConstantBlendSeek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matConstantInitBlend.c"

#include "shared/sdf/sdfMotion/sdfMotGenMatUvw.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matUvwSeek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matUvwBlendSeek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matUvwInitBlend.c"

#include "shared/sdf/sdfMotion/sdfMotGenMatMultiUvw.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matMultiUvwSeek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matMultiUvwBlendSeek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matMultiUvwInitBlend.c"

#include "shared/sdf/sdfMotion/sdfMotGenMatMultiDiffuse.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matMultiDiffuseSeek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matMultiDiffuseBlendSeek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matMultiDiffuseInitBlend.c"

#include "shared/sdf/sdfMotion/sdfMotGenMatUvw2.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matUvw2Seek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matUvw2BlendSeek.c"

void sdfMotCLBK_matUvw2InitBlend(void) {
}

#include "shared/sdf/sdfMotion/sdfMotGenMatMultiUvw2.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matMultiUvw2Seek.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_matMultiUvw2BlendSeek.c"

void sdfMotCLBK_matMultiUvw2InitBlend(void) {
}

#include "shared/sdf/sdfMotion/sdfMotGenMorphNode.c"

#include "shared/sdf/sdfMotion/sdfMotGenMorphInit.c"

#include "shared/sdf/sdfMotion/sdfMotGenMorph.c"

#include "shared/sdf/sdfMotion/sdfMotCLBK_morphSeek.c"

INCLUDE_ASM(const s32, "sdf/sdfMotion", sdfMotCLBK_morphBlendSeek);

#include "shared/sdf/sdfMotion/sdfMotCLBK_morphInitBlend.c"
