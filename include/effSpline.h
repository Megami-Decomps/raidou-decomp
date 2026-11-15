#ifndef EFFSPLINE_H
#define EFFSPLINE_H

#include "common.h"

typedef struct dds3Spline_t dds3Spline_t;

dds3Spline_t *dds3AllocSplineHandle(f32 (*points)[3], u32 count, s32 flattenEqualComponents);
void dds3FreeSplineHandle(dds3Spline_t *pSpline);

#endif
