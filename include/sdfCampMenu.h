#ifndef SDF_CAMP_MENU_H
#define SDF_CAMP_MENU_H

/* The field subscreen's menu. The macro expands to retail's code, so this
 * build is unchanged. A host that adds rows defines it first. */

/* Called with the menu's process once retail's rows, draw functions and
 * cancel sounds are set, before cmpMenuMaskSet and itfMenuStart. */
#ifndef SDF_CAMP_MENU_ROWS
#define SDF_CAMP_MENU_ROWS(pid) ((void)0)
#endif

#endif
