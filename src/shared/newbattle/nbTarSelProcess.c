#include "common.h"
#include "dds3KernelCore.h"

/* The menu process's data, known only as far as the menu count. */
typedef struct itfMenuProcessData_t {
#ifdef VERSION_RAIDOU1
    u8 unk000[0xB66];
#else
    u8 unk000[0xBAA];
#endif
    u8 menuNum;
} itfMenuProcessData_t;

u8 itfMenuGetMenuNum(dds3ProcessID_t *id) {
    return ((itfMenuProcessData_t *)dds3GetProcessData(id))->menuNum;
}
