#include "shared/sdf/sdf3D/decls.h"

void sdf3DMakeViewportPacket(sdf3DViewportPacket_t *aPacket, sdf3DScreenParam_t *aParam, void *aCamera) {
    aPacket->quadwords = 0xC;
    aPacket->command = 0x6C0BC000;
    aPacket->reservedWord = 0;
    SDF_VU0_LOADMATRIX(aCamera);
    SDF_VU0_STOREMATRIX(aPacket->matrixA);
    MultMatrixRev(aParam->TM);
    SDF_VU0_STOREMATRIX(aPacket->matrixB);
    SDF_VU0_LOADVECTOR(aParam->scale);
    SDF_VU0_STOREVECTOR(aPacket->vecA);
    SDF_VU0_LOADVECTOR(aParam->trans);
    SDF_VU0_STOREVECTOR(aPacket->vecB);
    SDF_VU0_LOADMATRIX(aCamera);
    ReverseMatrix();
    SDF_VU0_GET_ROW3();
    SDF_VU0_STOREVECTOR(aPacket->vecC);
    SDF_VU0_STOREVECTOR(aParam->cameraPos);
    aPacket->stmodCommand = SDF_VIF_ITOP_MATRIX;
    aPacket->mscalCommand = SDF_VIF_MSCAL_COMMAND;
    aPacket->unknownC8 = 0;
    aPacket->unknownCC = 0;
}
