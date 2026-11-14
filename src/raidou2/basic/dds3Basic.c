#include "common.h"
#include "raidou_alloc.h"

/* Declarations from smt3-decomp src/smt3/basic/dds3Basic.c */
#include "common.h"

#include "basic.h"

#include "ps2asm.h"

extern void *sdfChipAllocDirect(u32 aSize);

extern void Normalize(void);

extern void Mult(void);

extern void ToMatrix(void);

extern f32 fabsf(f32 value);

void dds3BackupPoseSet(dds3PoseSet_t *pPoseSet);

extern u32 dds3ObjectIdCounter;

void sdfChipFreeDirect(void *aPtr);

extern void dds3DeleteBasic(dds3Basic_t *basic);

extern dds3BasicFunction_t *dds3BasicFuncTablePtr[];

extern char dds3BasicPosLabel[];

extern char dds3BasicRotLabel[];

extern char dds3BasicSclLabel[];

dds3Basic_t *dds3AllocBasic(u32 type);

void dds3DeleteBasic(dds3Basic_t *basic);

s32 dds3InitPoseSet(dds3Basic_t *basic);

void dds3FreePoseSet(dds3Basic_t *basic);

void dds3OnPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag);

void dds3OffPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag);

s32 dds3CheckPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag);

/* vu0 routine: reset translation/rotation and set unit scale. */
void dds3BackupPoseSet(dds3PoseSet_t *pPoseSet);

void dds3SetRadiusPoseSet(dds3PoseSet_t *pose);

void dds3GetRadiusPoseSet(dds3Basic_t *basic, f32 r);

f32 func_0010DE98(dds3Basic_t *basic);

void dds3SetPosPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *pos);

void dds3SetRotPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *rot);

void dds3SetSclPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *scl);

/* vu0 routine: return the position in vf10 with homogeneous w = 1. */
void dds3GetPosPoseSet(dds3Basic_t *pBasic);

void dds3GetRotPoseSet(dds3Basic_t *pBasic);

/* vu0 routine: return scale in vf10. */
void func_0010DF70(dds3Basic_t *basic);

void dds3GetPosPoseSetMCR(dds3Basic_t *basic, const sdfVector4_t *offset);

void dds3AddRotPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *rot);

void dds3GetSclPoseSetMCR(dds3Basic_t *basic, const sdfVector4_t *scale);

/* vu0 routine: compose scale, quaternion rotation and translation in vf28-31. */
void dds3MakeMatrix(const sdfVector4_t *scale, const sdfVector4_t *quaternion,
                    const sdfVector4_t *transrate);

/* Load the cached matrix, rebuilding it only when a pose setter invalidated it. */
void dds3GetTMPoseSet(dds3Basic_t *pBasic);

s32 dds3ComparePosPoseSet(dds3PoseSet_t *left, dds3PoseSet_t *right);

/* vu0 routine: distance between the two positions, against the right radius. */
s32 dds3ComparePosPoseSet2(dds3PoseSet_t *left, dds3PoseSet_t *right);

s32 dds3CallBasicMainProc(dds3Basic_t *pBasic);

s32 dds3CallBasicDrawProc(dds3Basic_t *basic);

u32 dds3AllocObjectId(void);

void dds3SetUnitObjectId(dds3Basic_t *basic, char *name);

char *func_0010E270(dds3Basic_t *basic);

extern u32 dds3ObjectIdCounter;

extern dds3Basic_t *dds3TheWorld;
/* End of declarations from src/smt3/basic/dds3Basic.c */

INCLUDE_ASM(const s32, "basic/dds3Basic", dds3Add);

void dds3DeleteBasic(dds3Basic_t *basic)
{
    dds3BasicFunction_t *function;

    if (basic == NULL) {
        return;
    }
    function = basic->basicfunction;
    if (function != NULL && function->destructor != NULL) {
        ((void (*)(dds3Basic_t *))function->destructor)(basic);
    }
    if (basic->prev != NULL) {
        basic->prev->next = basic->next;
    }
    if (basic->next != NULL) {
        basic->next->prev = basic->prev;
    }
    sdfChipFreeDirect(basic);
}

s32 dds3InitPoseSet(dds3Basic_t *basic)
{
    dds3PoseSet_t *pose;

    if (basic == NULL) {
        return 0;
    }
    if (basic->poseset != NULL) {
        return 0;
    }
    pose = sdfChipAllocDirect(sizeof(dds3PoseSet_t));
    if (pose == NULL) {
        return 0;
    }
    pose->flag = POSESET_FLAG_UPDATEPOSE;
    dds3BackupPoseSet(pose);
    SDF_VU0_STOREIDENTITYVECTOR(&pose->plane);
    basic->poseset = pose;
    pose->frameFall = 0;
    pose->r = 0.0f;
    return 1;
}

void dds3FreePoseSet(dds3Basic_t *basic)
{
    if (basic != NULL && basic->poseset != NULL) {
        sdfChipFreeDirect(basic->poseset);
        basic->poseset = NULL;
    }
}

void dds3OnPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag)
{
    pPoseSet->flag |= flag;
}

void dds3OffPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag)
{
    pPoseSet->flag &= ~flag;
}

s32 dds3CheckPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag)
{
    return (pPoseSet->flag & flag) != 0;
}

/* vu0 routine: reset translation/rotation and set unit scale. */
void dds3BackupPoseSet(dds3PoseSet_t *pPoseSet)
{
    sdfVector4_t *position = &pPoseSet->pos;
    sdfVector4_t *rotation;
    sdfVector4_t *scale;

    SDF_VU0_STOREIDENTITYVECTOR(position);
    rotation = &pPoseSet->rot;
    SDF_VU0_STOREIDENTITYVECTOR(rotation);
    SDF_VU0_SETSCALE1VECTOR();
    scale = &pPoseSet->scl;
    SDF_VU0_STOREVECTOR(scale);
}

void dds3SetRadiusPoseSet(dds3PoseSet_t *pose)
{
    SDF_VU0_COPYVECTOR(&pose->workscl, &pose->scl);
    SDF_VU0_COPYVECTOR(&pose->workrot, &pose->rot);
    SDF_VU0_COPYVECTOR(&pose->workpos, &pose->pos);
}

void dds3GetRadiusPoseSet(dds3Basic_t *basic, f32 r)
{
    basic->poseset->r = r;
}

INCLUDE_ASM(const s32, "basic/dds3Basic", func_r1_00116F38);

INCLUDE_ASM(const s32, "basic/dds3Basic", dds3SetPosPoseSet_dds3Basic);

void dds3SetRotPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *rot)
{
    dds3PoseSet_t *pose = pBasic->poseset;
    sdfVector4_t *dst = &pose->rot;

    pose->flag = (pose->flag | POSESET_FLAG_UPDATEPOSE) & ~POSESET_FLAG_UPDATETM;
    SDF_VU0_COPYVECTOR(dst, rot);
}

void dds3SetSclPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *scl)
{
    dds3PoseSet_t *pose = pBasic->poseset;
    sdfVector4_t *dst = &pose->scl;

    pose->flag = (pose->flag | POSESET_FLAG_UPDATEPOSE) & ~POSESET_FLAG_UPDATETM;
    SDF_VU0_COPYVECTOR(dst, scl);
}

/* vu0 routine: return the position in vf10 with homogeneous w = 1. */
void dds3GetPosPoseSet(dds3Basic_t *pBasic)
{
    sdfVector4_t *position = &pBasic->poseset->pos;

    SDF_VU0_LOADVECTOR_BARRIER(position);
    SDF_VU0_VECTOR_W_SET_ONE();
}

void dds3GetRotPoseSet(dds3Basic_t *pBasic)
{
    sdfVector4_t *rotation = &pBasic->poseset->rot;

    SDF_VU0_LOADVECTOR_BARRIER(rotation);
    Normalize();
}

INCLUDE_ASM(const s32, "basic/dds3Basic", func_r1_00117010);

void dds3GetPosPoseSetMCR(dds3Basic_t *basic, const sdfVector4_t *offset)
{
    dds3PoseSet_t *pose = basic->poseset;
    sdfVector4_t *position = &pose->pos;

    pose->flag = (pose->flag | POSESET_FLAG_UPDATEPOSE) & ~POSESET_FLAG_UPDATETM;
    SDF_VU0_LOADVECTOR(position);
    SDF_VU0_LOADVECTOR_B_BARRIER(offset);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&pose->pos);
}

void dds3AddRotPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *rot)
{
    dds3PoseSet_t *pose = pBasic->poseset;

    pose->flag = (pose->flag | POSESET_FLAG_UPDATEPOSE) & ~POSESET_FLAG_UPDATETM;
    SDF_VU0_LOADVECTOR(&pose->rot);
    SDF_VU0_LOADVECTOR_B(rot);
    Mult();
    SDF_VU0_STOREVECTOR(&pose->rot);
}

void dds3GetSclPoseSetMCR(dds3Basic_t *basic, const sdfVector4_t *scale)
{
    dds3PoseSet_t *pose = basic->poseset;
    sdfVector4_t *src = &pose->scl;

    pose->flag = (pose->flag | POSESET_FLAG_UPDATEPOSE) & ~POSESET_FLAG_UPDATETM;
    SDF_VU0_LOADVECTOR(src);
    SDF_VU0_LOADVECTOR_B_BARRIER(scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&pose->scl);
}

/* vu0 routine: compose scale, quaternion rotation and translation in vf28-31. */
void dds3MakeMatrix(const sdfVector4_t *scale, const sdfVector4_t *quaternion,
                    const sdfVector4_t *transrate)
{
    SDF_VU0_LOADVECTOR(quaternion);
    ToMatrix();
    SDF_VU0_LOADVECTOR(scale);
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(transrate);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
}

/* Load the cached matrix, rebuilding it only when a pose setter invalidated it. */
void dds3GetTMPoseSet(dds3Basic_t *pBasic)
{
    dds3PoseSet_t *pose = pBasic->poseset;
    u32 flags = pose->flag;

    if (flags & POSESET_FLAG_UPDATETM) {
        SDF_VU0_LOADMATRIX(&pose->workTM);
    } else {
        pose->flag = flags | POSESET_FLAG_UPDATETM;
        dds3MakeMatrix(&pose->scl, &pose->rot, &pose->pos);
        SDF_VU0_STOREMATRIX(&pose->workTM);
    }
}

s32 dds3ComparePosPoseSet(dds3PoseSet_t *left, dds3PoseSet_t *right)
{
    f32 distance;
    f32 leftRadius;
    f32 rightRadius;

    SDF_VU0_LOADVECTOR(&left->pos);
    SDF_VU0_LOADVECTOR_B(&right->pos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(distance);
    leftRadius = left->r;
    rightRadius = right->r;
    if (fabsf(distance) - (leftRadius + rightRadius) < 0.0f) {
        return 1;
    }
    return 0;
}

/* vu0 routine: distance between the two positions, against the right radius. */
s32 dds3ComparePosPoseSet2(dds3PoseSet_t *left, dds3PoseSet_t *right)
{
    f32 distance;
    f32 rightRadius;

    SDF_VU0_LOADVECTOR(&left->pos);
    SDF_VU0_LOADVECTOR_B(&right->pos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(distance);
    distance = sdfFabs(distance);
    rightRadius = right->r;
    if (distance - rightRadius < 0.0f) {
        return 1;
    }
    return 0;
}

s32 dds3CallBasicMainProc(dds3Basic_t *pBasic)
{
    dds3BasicFunction_t *function;
    s32 result;

    result = 1;
    if (pBasic != NULL) {
        function = pBasic->basicfunction;
        if (function != NULL && function->mainprocuder != NULL) {
            result = ((s32 (*)(dds3Basic_t *))function->mainprocuder)(pBasic);
        }
    }
    return result;
}

s32 dds3CallBasicDrawProc(dds3Basic_t *basic)
{
    dds3BasicFunction_t *function;
    s32 result;

    result = 1;
    if (basic != NULL) {
        function = basic->basicfunction;
        if (function != NULL && function->drawprocuder != NULL) {
            result = ((s32 (*)(dds3Basic_t *))function->drawprocuder)(basic);
        }
    }
    return result;
}
