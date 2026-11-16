#include "common.h"
#include "effect.h"
#include "sdfMath.h"
#include "effBlur_Filter.h"

void dds3CallCos(const dds3Effect_Blur_t *source, effBlurPrimTriStrip_t *quad, u8 fixedPointCoordinates)
{
    f32 centerX, centerY, scale, cosine, sine;
    f32 scaledU, scaledV, u, v;

    quad->r = ((u8 *)&source->abgr)[0];
    quad->g = ((u8 *)&source->abgr)[1];
    quad->b = ((u8 *)&source->abgr)[2];
    quad->a = ((u8 *)&source->abgr)[3];
    quad->a = quad->a * effBlurGetBlurValid();
    if (fixedPointCoordinates == 0) {
        quad->vtx[0].x = (source->xy[0][0] << 4) + 0x7000;
        quad->vtx[0].y = (source->xy[0][1] << 3) + 0x7900;
        quad->vtx[1].x = quad->vtx[0].x;
        quad->vtx[1].y = (source->xy[1][1] << 3) + 0x7900;
        quad->vtx[2].x = (source->xy[1][0] << 4) + 0x7000;
        quad->vtx[2].y = quad->vtx[0].y;
        quad->vtx[3].x = quad->vtx[2].x;
        quad->vtx[3].y = quad->vtx[1].y;
        quad->vtx[0].s = source->xy[0][0] * (1.0f / 512.0f);
        quad->vtx[0].t = source->xy[0][1] * (1.0f / 512.0f);
        quad->vtx[1].s = quad->vtx[0].s;
        quad->vtx[1].t = source->xy[1][1] * (1.0f / 512.0f);
        quad->vtx[2].s = source->xy[1][0] * (1.0f / 512.0f);
        quad->vtx[2].t = quad->vtx[0].t;
        quad->vtx[3].s = quad->vtx[2].s;
        quad->vtx[3].t = quad->vtx[1].t;
    } else {
        quad->vtx[0].x = source->xy[0][0] + 0x7000;
        quad->vtx[0].y = source->xy[0][1] + 0x7900;
        quad->vtx[1].x = quad->vtx[0].x;
        quad->vtx[1].y = source->xy[1][1] + 0x7900;
        quad->vtx[2].x = source->xy[1][0] + 0x7000;
        quad->vtx[2].y = quad->vtx[0].y;
        quad->vtx[3].x = quad->vtx[2].x;
        quad->vtx[3].y = quad->vtx[1].y;
        quad->vtx[0].s = source->xy[0][0] * (1.0f / 8192.0f);
        quad->vtx[0].t = source->xy[0][1] * (1.0f / 4096.0f);
        quad->vtx[1].s = quad->vtx[0].s;
        quad->vtx[1].t = source->xy[1][1] * (1.0f / 4096.0f);
        quad->vtx[2].s = source->xy[1][0] * (1.0f / 8192.0f);
        quad->vtx[2].t = quad->vtx[0].t;
        quad->vtx[3].s = quad->vtx[2].s;
        quad->vtx[3].t = quad->vtx[1].t;
    }
    quad->vtx[0].z = 0;
    quad->vtx[0].adc = 0;
    quad->vtx[1].z = 0;
    quad->vtx[1].adc = 0;
    quad->vtx[2].z = 0;
    quad->vtx[2].adc = 0;
    quad->vtx[3].z = 0;
    quad->vtx[3].adc = 0;
    if (fixedPointCoordinates == 0) {
        u = source->center[0] + 256.0f;
        v = source->center[1] + 224.0f;
        centerX = u * (1.0f / 512.0f);
        centerY = v * (1.0f / 448.0f);
    } else {
        u = source->center[0] + 4096.0f;
        v = source->center[1] + 1792.0f;
        centerX = u * (1.0f / 8192.0f);
        centerY = v * (1.0f / 3584.0f);
    }
    scale = source->scale;
    cosine = sdfCos(source->rotate);
    sine = sdfSin(source->rotate);
    scaledU = (quad->vtx[0].s - centerX) * scale;
    scaledV = (quad->vtx[0].t - centerY) * scale;
    u = scaledU * cosine - scaledV * sine;
    v = scaledU * sine + scaledV * cosine;
    quad->vtx[0].s = u + centerX;
    quad->vtx[0].t = v + centerY;
    scaledU = (quad->vtx[1].s - centerX) * scale;
    scaledV = (quad->vtx[1].t - centerY) * scale;
    u = scaledU * cosine - scaledV * sine;
    v = scaledU * sine + scaledV * cosine;
    quad->vtx[1].s = u + centerX;
    quad->vtx[1].t = v + centerY;
    scaledU = (quad->vtx[2].s - centerX) * scale;
    scaledV = (quad->vtx[2].t - centerY) * scale;
    u = scaledU * cosine - scaledV * sine;
    v = scaledU * sine + scaledV * cosine;
    quad->vtx[2].s = u + centerX;
    quad->vtx[2].t = v + centerY;
    scaledU = (quad->vtx[3].s - centerX) * scale;
    scaledV = (quad->vtx[3].t - centerY) * scale;
    u = scaledU * cosine - scaledV * sine;
    v = scaledU * sine + scaledV * cosine;
    quad->vtx[3].s = u + centerX;
    quad->vtx[3].t = v + centerY;
}
