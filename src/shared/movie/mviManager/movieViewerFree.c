#include "shared/movie/mviManager/decls.h"

void movieViewerFree(void)
{
    void *chip = movieViewerWork.chip;

    if (chip != NULL) {
        do {
            void *next = *(void **)chip;

            sdfChipFreeDirect(chip);
            chip = next;
        } while (chip != NULL);
        movieViewerWork.chip = NULL;
        movieViewerWork.top = 0;
        movieViewerWork.cursor = 0;
        movieViewerWork.unk_0C = 0;
    }
}
