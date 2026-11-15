#ifndef CTN_H
#define CTN_H

#include "common.h"

struct ctnCitizen_t;
struct ctnExit_t;
struct ctnGrid_t;
struct ctnInput_t;

struct ctnCitizen_t *AddCitizen(void);
s32 CheckExit(struct ctnCitizen_t *pCitizen);
void EnterBlock(struct ctnCitizen_t *pCitizen, struct ctnGrid_t *pGrid);
struct ctnCitizen_t *FindAhead(struct ctnCitizen_t *pCitizen, f32 maxDist);
struct ctnCitizen_t *FindAheadList(struct ctnCitizen_t *pCitizen, f32 maxDist, s32 citizenNum, struct ctnCitizen_t *ctnList);
struct ctnCitizen_t *FindHeadOn(struct ctnCitizen_t *pCitizen, f32 maxDist, s32 citizenNum, struct ctnCitizen_t *ctnList);
struct ctnGrid_t *FreeGridBeside(f32 ox, f32 oy, struct ctnCitizen_t *pCitizen);
f32 GetClassVelocity(struct ctnCitizen_t *pCitizen);
void InitFromFile(s32 id, s32 floor);
void InitWork(void);
void KillCitizen(struct ctnCitizen_t *pCitizen);
void KillCitizenFriend(struct ctnCitizen_t *pCitizen);
void MoveFollower(struct ctnCitizen_t *pCitizen);
void SetHeadOn(struct ctnCitizen_t *pCitizen, s32 maxDist);
void SetPref(struct ctnCitizen_t *pCitizen);
void SetWallInfo(struct ctnCitizen_t *pCitizen);
void ShootCitizen(struct ctnExit_t *pExit);
void SimulateSetFlags(void);
void UpdateCitizenPos(struct ctnCitizen_t *pCitizen);
void UpdateCitizenRotate(struct ctnCitizen_t *pCitizen);
void UpdateCitizenVelocity(struct ctnCitizen_t *pCitizen);
void UpdateTargetVelocity(struct ctnCitizen_t *pCitizen);
struct ctnGrid_t *ctnGetGrid(s32 x, s32 y);
void ctnLoadDataIDFloor(s32 id, s32 floor);
void ctnMemFree(void);
void ctnUpdate(struct ctnInput_t *pInput);

#endif
