#ifndef CODE_R1_00128378_H
#define CODE_R1_00128378_H

#include "common.h"
#include "basic.h"

void fldAiboCashLoad_Block(void);
void fldCtnGetSub(dds3Basic_t *pBasic);
s32 fldPlayerALoadEndCheck(void);
void fldPlayerALoadStart(void);
void fldRegistUnitPad(void);
void fldSetUnitSub2(void);
s32 mapGetCircleCircleH(f32 x1, f32 y1, f32 r1, f32 x2, f32 y2, f32 r2, f32 *x3, f32 *y3);
void mapGetEyeTgtRot(f32 eyex, f32 eyez, f32 tgtx, f32 tgtz);
f32 mapGetEyeTgt_Rot_OfsPos(f32 eyex, f32 eyez, f32 tgtx, f32 tgtz, f32 olen, f32 *retx, f32 *retz);
dds3Basic_t *mapGetPlayersBasic(s32 type);
void mapGetTaniVector(f32 vx0, f32 vy0, f32 *vx1, f32 *vy1);
void mapRaidouBackUpPosReset(void);
void mapRndEncParamSet(void);
void mapSetPlayersVisible(s32 r_onoff, s32 n_onoff, s32 d_onoff);

#endif
