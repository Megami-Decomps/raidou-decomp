#ifndef SDFVU0_H
#define SDFVU0_H

#include "common.h"
#include "sdf_types.h"

void DiagMatrix(void);
void LoadIdentity(void);
void LookAtMatrix(const void *aTarget, const void *aEye, const void *aDown);
void MultMatrixNoArg(void);
void MultMatrixRev(const void *aMatrix);
void MultMatrixRevNoArg(void);
void ReverseMatrix(void);
void RotateMatrixX(f32 aAngle);
void RotateMatrixY(f32 aAngle);
void SetRotateMatrixAxis(f32 aAngle, const sdfVector3_t* aAxis);
void SetRotateMatrixX(f32 aAngle);
void SetRotateMatrixXYZ(const sdfVector3_t *aAngles);
void SetRotateMatrixX_B(f32 aAngle);
void SetRotateMatrixY(f32 aAngle);
void SetRotateMatrixY_B(f32 aAngle);
void SetRotateMatrixZ(f32 aAngle);
void SetRotateMatrixZ_B(f32 aAngle);

#endif
