#ifndef SDFQUAT_H
#define SDFQUAT_H

#include "common.h"

f32 EulerAngleAxisY(void);
void FromEuler(f32 aRotateX, f32 aRotateY, f32 aRotateZ);
void FromRotation(f32 aAngle);
void FromRotation_B(f32 aAngle);
void Lerp(f32 aRatio);
void Mult(void);
extern f32 sdfUnitXVector[4];
void Normalize(void);
void Slerp(f32 aRatio);
void ToMatrix(void);

#endif
