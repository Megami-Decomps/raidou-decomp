/* Declarations of smt3-decomp src/smt3/sdf/sdfVu0.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFVU0_H
#define SHARED_SDF_SDFVU0_H

#include "common.h"

#include "ps2asm.h"

#include "sdf_lighting.h"

extern f32 cosf(f32 angle);

extern f32 sinf(f32 angle);

void LoadIdentity(void);

void DiagMatrix(void);

void ReverseMatrix(void);

void InversMatrix(void);

void SetRotateMatrixX(f32 aAngle);

void SetRotateMatrixY(f32 aAngle);

void SetRotateMatrixZ(f32 aAngle);

void StoreRotateMatrixAxis(f32 aAngle, const sdfVector3_t *aAxis, sdfMatrix4_t *aMtx);

void SetRotateMatrixAxis(f32 aAngle, const sdfVector3_t* aAxis);

void SetRotateMatrixX_B(f32 aAngle);

void SetRotateMatrixY_B(f32 aAngle);

void SetRotateMatrixZ_B(f32 aAngle);

void SetRotateMatrixAxis_B(const sdfVector3_t* aAngle, f32 aAxis);

void RotateMatrixX(f32 aAngle);

void RotateMatrixY(f32 aAngle);

void RotateMatrixZ(f32 aAngle);

void SetRotateMatrixXYZ(const sdfVector3_t *aAngles);

void MultMatrixNoArg(void);

void MultMatrixRevNoArg(void);

void MultMatrix(void *aMatrix);

void MultMatrixRev(const void *aMatrix);

void LookAtMatrix(const void *aTarget, const void *aEye, const void *aDown);

void LoadMatrix(const void *aMatrix);

void LoadMatrix_B(const void *aMatrix);

void StoreMatrix(void *aMatrix);

void StoreMatrix_B(void *aMatrix);

void CopyMatrixAtoB(void);

void CopyMatrixAtoC(void);

void CopyMatrixBtoA(void);

void CopyMatrixBtoC(void);

void CopyMatrixCtoA(void);

void CopyMatrixCtoB(void);

void LoadIdentity(void);

void LoadIdentity_B(void);

/* vu0 routine: store the unit matrix built from vf0 by MMI lane shuffles */
void StoreIdentity(f32 *aMatrix);

/* libvu0: sceVu0TransposeMatrix, register form (vf28-vf31 in and out) */
void DiagMatrix(void);

/* vu0 routine: rigid inverse of vf28-vf31 (transpose the 3x3, translation = -(R^T * t)) */
void ReverseMatrix(void);

/* vu0 routine: inverse of vf28-vf31 with per-axis scale removed (transpose, rows / |row|^2, translation = -(R^T * t)) */
void InversMatrix(void);

void SetRotateMatrixX(f32 aAngle);

void SetRotateMatrixY(f32 aAngle);

void SetRotateMatrixZ(f32 aAngle);

void StoreRotateMatrixAxis(f32 aAngle, const sdfVector3_t *aAxis, sdfMatrix4_t *aMtx);

void SetRotateMatrixAxis(f32 aAngle, const sdfVector3_t* aAxis);

void SetRotateMatrixX_B(f32 aAngle);

void SetRotateMatrixY_B(f32 aAngle);

void SetRotateMatrixZ_B(f32 aAngle);

void SetRotateMatrixAxis_B(const sdfVector3_t* aAngle, f32 aAxis);

/* vu0 routine: rotate the vf29/vf30 pair by angle (vf29 = vf29*c + vf30*s, vf30 = vf30*c - vf29*s) */
void RotateMatrixX(f32 aAngle);

/* vu0 routine: rotate the vf28/vf30 pair by angle (vf28 = vf28*c - vf30*s, vf30 = vf30*c + vf28*s) */
void RotateMatrixY(f32 aAngle);

/* vu0 routine: rotate the vf28/vf29 pair by angle (vf28 = vf28*c + vf29*s, vf29 = vf29*c - vf28*s) */
void RotateMatrixZ(f32 aAngle);

void SetRotateMatrixXYZ(const sdfVector3_t *aAngles);

/* vu0 routine: vf28-vf31 = vf24-vf27 * vf28-vf31 (4x4 product) */
void MultMatrixNoArg(void);

/* vu0 routine: vf28-vf31 = vf28-vf31 * vf24-vf27 (4x4 product) */
void MultMatrixRevNoArg(void);

/* vu0 routine: vf28-vf31 = vf20-vf23 * vf28-vf31 (4x4 product) */
void MultMatrixNoArg_C(void);

/* vu0 routine: vf28-vf31 = vf28-vf31 * vf20-vf23 (4x4 product) */
void MultMatrixRevNoArg_C(void);

void MultMatrix(void *aMatrix);

void MultMatrixRev(const void *aMatrix);

void ApplyMatrix(void *aReVector, const void *aVector);

f32 InnerProduct(const void *a, const void *b);

void OuterProduct(void *aReVector, const void *aVectorA, const void *aVectorB);

/* vu0 routine: look-at basis in vf28-vf31 (forward, right, up, eye), then its rigid inverse */
void LookAtMatrix(const void *aTarget, const void *aEye, const void *aDown);

#endif
