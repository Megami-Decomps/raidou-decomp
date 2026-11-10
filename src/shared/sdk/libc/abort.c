#include "shared/sdk/libc/decls.h"

void abort(void) {
    while (1) {
        raise(6);
        func_001001C8(1);
    }
}
