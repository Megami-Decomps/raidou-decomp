#include "shared/sdk/libc/decls.h"

void _cleanup_r(_reent *r) {
    _fwalk(r, fflush);
}
