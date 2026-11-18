#ifndef SDF_MENU_ANCHOR_H
#define SDF_MENU_ANCHOR_H

/* Which screen edge an itf menu keeps to. Retail lays out every menu in the
 * 4:3 frame, so each macro expands to nothing and this build is unchanged. A
 * host that draws a wider frame defines them first. */

typedef enum SDF_MENU_ANCHOR {
    SDF_MENU_ANCHOR_CENTRE,
    SDF_MENU_ANCHOR_LEFT,
    SDF_MENU_ANCHOR_RIGHT
} SDF_MENU_ANCHOR;

/* Sets the anchor of the menu that pid runs; a new menu's is SDF_MENU_ANCHOR_CENTRE. */
#ifndef SDF_MENU_SET_ANCHOR
#define SDF_MENU_SET_ANCHOR(pid, anchor) ((void)0)
#endif

/* Around one menu's draw, wk its Menu_t: what is drawn between keeps its anchor. */
#ifndef SDF_MENU_DRAW_BEGIN
#define SDF_MENU_DRAW_BEGIN(wk) ((void)0)
#endif
#ifndef SDF_MENU_DRAW_END
#define SDF_MENU_DRAW_END(wk) ((void)0)
#endif

#endif
