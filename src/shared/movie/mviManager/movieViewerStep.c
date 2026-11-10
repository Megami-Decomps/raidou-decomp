#include "shared/movie/mviManager/decls.h"

/* The entry the cursor sits on, walked from the head. */
void *movieViewerStep(void)
{
    movieViewerChip_t *chip = movieViewerWork.chip;
    s32 remaining = movieViewerWork.cursor;

    if (chip != NULL && remaining > 0) {
        do {
            chip = chip->next;
            remaining--;
        } while (chip != NULL && remaining > 0);
    }
    return chip;
}
