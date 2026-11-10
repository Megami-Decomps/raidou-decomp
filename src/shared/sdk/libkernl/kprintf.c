#include "shared/sdk/libkernl/decls.h"

void kprintf(const char *format, ...)
{
    kernelVaList_t arguments;

    arguments = (kernelVaList_t)__builtin_next_arg(format) - 7 * sizeof(long);
    _printf(format, arguments);
}
