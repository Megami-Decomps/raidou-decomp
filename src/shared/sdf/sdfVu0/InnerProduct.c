#include "shared/sdf/sdfVu0/decls.h"

f32 InnerProduct(const void *a, const void *b) {
    f32 product;

    SDF_VU0_LOADVECTOR(a);
    SDF_VU0_LOADVECTOR_B(b);
    SDF_VU0_INNERPRODUCT(product);
    return product;
}
