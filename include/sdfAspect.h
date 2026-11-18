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

/* The field view's crop. A host may scale the field picture and the 3D scene
 * together about the screen's centre, so a picture narrower than its screen
 * fills it. VIEW_UPDATE gives it the picture's left x and drawn width once a
 * frame; VIEW_X, VIEW_Y and VIEW_SIZE place the picture's 2D, VIEW_CAMERA
 * scales the field camera, and VIEW_TRANS gives the scroll the camera's
 * offset as it was before the crop. */
#ifndef SDF_ASPECT_VIEW_UPDATE
#define SDF_ASPECT_VIEW_UPDATE(left, width)
#endif
#ifndef SDF_ASPECT_VIEW_X
#define SDF_ASPECT_VIEW_X(x) (x)
#endif
#ifndef SDF_ASPECT_VIEW_Y
#define SDF_ASPECT_VIEW_Y(y) (y)
#endif
#ifndef SDF_ASPECT_VIEW_SIZE
#define SDF_ASPECT_VIEW_SIZE(size) (size)
#endif
#ifndef SDF_ASPECT_VIEW_CAMERA
#define SDF_ASPECT_VIEW_CAMERA(camera)
#endif
#ifndef SDF_ASPECT_VIEW_TRANS
#define SDF_ASPECT_VIEW_TRANS(trans) (trans)
#endif

#endif
