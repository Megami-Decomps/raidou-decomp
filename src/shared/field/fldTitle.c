#include "common.h"
#include "mapData.h"
#include "mapLbWork.h"
#include "sdfTex.h"

void fldTitleMini_Shutdown(void) {
    if (fldTitleMiniTex2 != NULL) {
        sdfTexReleaseReference(fldTitleMiniTex2);
        fldTitleMiniTex2 = NULL;
    }
    if (fldTitleMiniWork2 != NULL) {
        sdfTexReleaseReference(fldTitleMiniWork2);
        fldTitleMiniWork2 = NULL;
    }
}
