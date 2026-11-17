#include "common.h"

/* The menu process's data, known only as far as the menu count. */
typedef struct itfMenuProcessData_t {
#ifdef VERSION_RAIDOU1
    u8 unk000[0xB66];
#else
    u8 unk000[0xBAA];
#endif
    u8 menuNum;
} itfMenuProcessData_t;

extern itfMenuProcessData_t *dds3GetProcessData(void);

/* The callee takes no argument here, unlike the dds3GetProcessData(task) other units declare. */
u8 itfMenuGetMenuNum(void) {
    return dds3GetProcessData()->menuNum;
}
