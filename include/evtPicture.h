#ifndef EVTPICTURE_H
#define EVTPICTURE_H

#include "common.h"
#include "kernel.h"

void evtKillPictureProcess(dds3ProcessID_t *id);
void evtPictureDrawOff(dds3ProcessID_t *id);
void evtPictureDrawOn(dds3ProcessID_t *id);

#endif
