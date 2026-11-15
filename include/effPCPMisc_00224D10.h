#ifndef EFFPCPMISC_00224D10_H
#define EFFPCPMISC_00224D10_H

#include "common.h"

void fileReadRecordSecondVector(u8 *recordBytes, void *dst);
void fileReadVectorPtr20(u8 *recordBytes, void *dst);
void fileSetRecordSecondVector(void *recordBytes, const void *src);
void mnuRecordSetVector(void *recordBytes, const void *src);

#endif
