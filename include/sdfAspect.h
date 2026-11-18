#ifndef SDF_ASPECT_H
#define SDF_ASPECT_H

/* Camera limits chosen so a 4:3 view stays inside the modelled set. Each macro
 * expands to retail's code, so this build is unchanged. A host that draws a
 * wider view defines them first and narrows each limit by the extra width. */

/* Clamps a camera's horizontal coordinate x to [lo, hi]. */
#ifndef SDF_ASPECT_CLAMP_X
#define SDF_ASPECT_CLAMP_X(x, lo, hi)                                                \
    do {                                                                            \
        if ((x) < (lo)) {                                                           \
            (x) = (lo);                                                             \
        }                                                                           \
        if ((x) > (hi)) {                                                           \
            (x) = (hi);                                                             \
        }                                                                           \
    } while (0)
#endif

/* The 2D screen's left and right edge in x, for the field background's tile
 * culling and the fills that must cover the whole screen. A host defines
 * them as its own edges, an int each. */
#ifndef SDF_ASPECT_LEFT
#define SDF_ASPECT_LEFT 0
#endif
#ifndef SDF_ASPECT_RIGHT
#define SDF_ASPECT_RIGHT 512
#endif

/* The x a scrolled field picture of width w must reach on the left and on the
 * right: its scroll is clamped so the picture covers [left, right]. */
#ifndef SDF_ASPECT_PICTURE_LEFT
#define SDF_ASPECT_PICTURE_LEFT(w) 0.0f
#endif
#ifndef SDF_ASPECT_PICTURE_RIGHT
#define SDF_ASPECT_PICTURE_RIGHT(w) 512.0f
#endif

#endif
