#include "shared/facility/fclShare/decls.h"

s32 fclLoadTexture(char *pFileName)
{
    void *data;
    s32 size;
    void *file;
    s32 handle;

    file = sdfDevLoadFileEx(pFileName, (u32 *)&data, &size);
    handle = sdfTexSet(data);
    sdfMemFreeDirect(file);
    return handle;
}
