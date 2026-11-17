#include "common.h"
#include "code_r1_001CC1D0.h"
#include "sdk_libc.h"

void jmsCallMemset2(mnuFadeFields_t *menu) {
    memset(menu, 0, sizeof(mnuWindowContainer_t));
    menu->hasResourceCopy = 0;
    menu->previousProgress = 0x200;
    menu->currentWindow = NULL;
    menu->currentProgress = 0;
}

void jmsLeaf27(mnuWindowContainer_t *windowAddress, mnuFadeFields_t *fadeWork) {
    mnuWindowContainer_t *window = windowAddress;
    mnuFadeFields_t *work = fadeWork;

    if (work->currentWindow != NULL) {
        work->previousWindow = *work->currentWindow;
        work->previousVisibleCount = work->currentWindow->list->visibleCount;
        if (work->currentWindow->resource != NULL) {
            work->hasResourceCopy = 1;
            work->savedResource = *work->currentWindow->resource;
        } else {
            work->hasResourceCopy = 0;
        }
        work->previousProgress = 0;
    } else {
        work->previousProgress = 512;
    }
    if (window != NULL) {
        window->panel.fade = 512;
        window->flags |= 4;
    }
    work->currentWindow = window;
    work->currentProgress = 0;
}

INCLUDE_ASM(const s32, "code_r1_0034F068", jmsLeaf28);

INCLUDE_ASM(const s32, "code_r1_0034F068", jmsFunc48);
