#include "shared/sdk/libkernl/decls.h"

void scePrintf(const char *format, ...)
{
    kernelVaList_t arguments;
    void (*previous)(s32);

    previous = printfPutchar;
    printfPutchar = deci2Putchar;
    arguments = (kernelVaList_t)__builtin_next_arg(format) - 7 * sizeof(long);
    _printf(format, arguments);
    printfPutchar = previous;
}
