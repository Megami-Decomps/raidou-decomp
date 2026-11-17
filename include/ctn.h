#ifndef CTN_H
#define CTN_H

#include "common.h"
#include "sdf_types.h"

struct ctnExit_t;
struct ctnGrid_t;
struct ctnInput_t;

typedef struct ctnTargetInfo_t {
    f32 moveDir[2];
    f32 moveNormal[2];
    s32 blockIndex;
    s32 leftWallIndex;
    s32 rightWallIndex;
    s32 resv;
} ctnTargetInfo_t;

typedef struct ctnCitizen_t {
    sdfVector4_t rotateQuat;
    s32 fExist;
    f32 x;
    f32 y;
    f32 vx;
    f32 vy;
    f32 targetVx;
    f32 targetVy;
    f32 vleng;
    f32 dirX;
    f32 dirY;
    f32 normalX;
    f32 normalY;
    f32 offCenter;
    f32 leftWallDist;
    f32 rightWallDist;
    s32 index;
    s32 sliceIndex;
    s32 attr;
    u8 exitIndex;
    s8 pathPref;
    u8 classType;
    s8 followType;
    u8 friendNum;
    u8 blockIndex;
    u8 belongIndex;
    u8 evadeType; /* CTN_EVADE */
    ctnTargetInfo_t *pTargetInfo;
    struct ctnCitizen_t *pAhead;
    struct ctnCitizen_t *pBehind;
    struct ctnCitizen_t *pHeadOn;
    struct ctnCitizen_t *pFriends[4];
    s32 playerEvadeArray[7];
    s32 resv0;
    s32 objectEvadeArray[7]; /* 0xA0 */
    s32 objectDir;
    s32 aheadEvadeArray[7];
    s32 spdType;
    s32 dir;
    f32 angle;
    f32 oldX;
    f32 oldY;
} ctnCitizen_t;
typedef char ctnCitizenSizeCheck[sizeof(ctnCitizen_t) == 0xF0 ? 1 : -1];

typedef struct ctnWork_t {
    u8 unk0[0x3A0];
    ctnCitizen_t citizen[256]; /* 0x3A0 */
} ctnWork_t;

extern ctnWork_t *Work_ctn;

typedef struct ctnObject_t {
    f32 center[2];
    f32 radius;
    s32 resv;
} ctnObject_t;

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
f32 ctnFindAheadListSub(f32 ax, f32 ay, f32 bx, f32 by);
void ctnFunc3(struct ctnCitizen_t *pCitizen, f32 *pos, f32 *leftWallDist, f32 *rightWallDist);
void ctnRefWorkCtn8(struct ctnCitizen_t *pCitizen, f32 *leftWallDist, f32 *rightWallDist);
void ctnFunc4(struct ctnCitizen_t *pCitizen, struct ctnCitizen_t *pOther, f32 *leftWallDist, f32 *rightWallDist);

extern const char stfProcName[];

#endif
