#ifndef FCLMSG_H
#define FCLMSG_H

#include "common.h"
#include "kernel.h"

void Create_fclMsg(void);
void Delete_fclMsg(void);
dds3ProcessFunc_t Process_fclMsg(dds3ProcessID_t *PID);
void SetMessagePos(u8 a_pos);
s32 Update_fclMsg(void);
s32 fclGetSelect(void);
void fclLinkData(u8 *a_pDatMsg);
void fclLinkDataEx(dds3ProcessID_t *PID, u8 *a_pDatMsg);
dds3ProcessID_t *fclProcessStart(void);
void fclRefMainPIDFclMsg(s32 a_mode);
extern dds3ProcessID_t *g_mainPID_fclMsg;
void fclResetEx(dds3ProcessID_t *PID);
void fclSetClsModeEx(dds3ProcessID_t *PID, s32 a_mode);
void fclSetMessagePosEx(dds3ProcessID_t *PID, u8 a_pos);
s32 fclUpdateEx(dds3ProcessID_t *PID);

#endif
