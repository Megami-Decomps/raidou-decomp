#include "shared/basic/dds3EffectObjectBasic/decls.h"

dds3Basic_t *dds3AddESPEffectObjectBasic(const char *name, void *pos, void *rot, f32 r)
{
    return AddESPEffect(name, pos, rot, r);
}
