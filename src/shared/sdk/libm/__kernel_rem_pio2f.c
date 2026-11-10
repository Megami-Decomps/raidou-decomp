#include "shared/sdk/libm/decls.h"

s32 __kernel_rem_pio2f(f32 *x, f32 *y, s32 e0, s32 nx, s32 precision,
                       const s32 *ipio2)
{
    s32 jz, jx, jv, jp, jk, carry, n, iq[20], i, j, k, m, q0, ih;
    f32 z, fw, f[20], fq[20], q[20];

    jk = D_0054B348[precision];
    jp = jk;
    jx = nx - 1;
    jv = (e0 - 3) / 8;
    if (jv < 0) jv = 0;
    q0 = e0 - 8 * (jv + 1);

    j = jv - jx;
    m = jx + jk;
    for (i = 0; i <= m; i++, j++) {
        f[i] = j < 0 ? 0.0f : (f32)ipio2[j];
    }
    for (i = 0; i <= jk; i++) {
        fw = 0.0f;
        for (j = 0; j <= jx; j++) {
            fw += x[j] * f[jx + i - j];
        }
        q[i] = fw;
    }

    jz = jk;
recompute:
    z = q[jz];
    for (i = 0, j = jz; j > 0; i++, j--) {
        fw = (f32)(s32)(0x1.000000p-8f * z);
        iq[i] = (s32)(z - 256.0f * fw);
        z = q[j - 1] + fw;
    }

    z = scalbnf(z, q0);
    z -= 8.0f * floorf(z * 0.125f);
    n = (s32)z;
    z -= (f32)n;
    ih = 0;
    if (q0 > 0) {
        i = iq[jz - 1] >> (8 - q0);
        n += i;
        iq[jz - 1] -= i << (8 - q0);
        ih = iq[jz - 1] >> (7 - q0);
    } else if (q0 == 0) {
        ih = iq[jz - 1] >> 8;
    } else if (z >= 0.5f) {
        ih = 2;
    }

    if (ih > 0) {
        n++;
        carry = 0;
        for (i = 0; i < jz; i++) {
            j = iq[i];
            if (carry == 0) {
                if (j != 0) {
                    carry = 1;
                    iq[i] = 256 - j;
                }
            } else {
                iq[i] = 255 - j;
            }
        }
        if (q0 > 0) {
            switch (q0) {
            case 1: iq[jz - 1] &= 0x7F; break;
            case 2: iq[jz - 1] &= 0x3F; break;
            }
        }
        if (ih == 2) {
            z = 1.0f - z;
            if (carry != 0) z -= scalbnf(1.0f, q0);
        }
    }

    if (z == 0.0f) {
        j = 0;
        for (i = jz - 1; i >= jk; i--) j |= iq[i];
        if (j == 0) {
            for (k = 1; iq[jk - k] == 0; k++) {
            }
            for (i = jz + 1; i <= jz + k; i++) {
                f[jx + i] = (f32)ipio2[jv + i];
                fw = 0.0f;
                for (j = 0; j <= jx; j++) {
                    fw += x[j] * f[jx + i - j];
                }
                q[i] = fw;
            }
            jz += k;
            goto recompute;
        }
    }

    if (z == 0.0f) {
        jz--;
        q0 -= 8;
        while (iq[jz] == 0) {
            jz--;
            q0 -= 8;
        }
    } else {
        z = scalbnf(z, -q0);
        if (z >= 256.0f) {
            fw = (f32)(s32)(0x1.000000p-8f * z);
            iq[jz] = (s32)(z - 256.0f * fw);
            jz++;
            q0 += 8;
            iq[jz] = (s32)fw;
        } else {
            iq[jz] = (s32)z;
        }
    }

    fw = scalbnf(1.0f, q0);
    for (i = jz; i >= 0; i--) {
        q[i] = fw * (f32)iq[i];
        fw *= 0x1.000000p-8f;
    }
    for (i = jz; i >= 0; i--) {
        fw = 0.0f;
        for (k = 0; k <= jp && k <= jz - i; k++) {
            fw += D_0054B358[k] * q[i + k];
        }
        fq[jz - i] = fw;
    }

    switch (precision) {
    case 0:
        fw = 0.0f;
        for (i = jz; i >= 0; i--) fw += fq[i];
        y[0] = ih == 0 ? fw : -fw;
        break;
    case 1:
    case 2:
        fw = 0.0f;
        for (i = jz; i >= 0; i--) fw += fq[i];
        y[0] = ih == 0 ? fw : -fw;
        fw = fq[0] - fw;
        for (i = 1; i <= jz; i++) fw += fq[i];
        y[1] = ih == 0 ? fw : -fw;
        break;
    case 3:
        for (i = jz; i > 0; i--) {
            fw = fq[i - 1] + fq[i];
            fq[i] += fq[i - 1] - fw;
            fq[i - 1] = fw;
        }
        for (i = jz; i > 1; i--) {
            fw = fq[i - 1] + fq[i];
            fq[i] += fq[i - 1] - fw;
            fq[i - 1] = fw;
        }
        fw = 0.0f;
        for (i = jz; i >= 2; i--) fw += fq[i];
        if (ih == 0) {
            y[0] = fq[0];
            y[1] = fq[1];
            y[2] = fw;
        } else {
            y[0] = -fq[0];
            y[1] = -fq[1];
            y[2] = -fw;
        }
        break;
    }
    return n & 7;
}
