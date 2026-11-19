#include "common.h"
#include "dds3Randomize.h"
#include "sdfMotion.h"
#include "ps2asm.h"
#include "sdfQuat.h"
#include "dds3UnitObjectBasic.h"
#include "mdl.h"
#include "dds3ObjectBase.h"
#include "dds3obj.h"
#include "fld.h"
#include "motion.h"
#include "fileManager.h"
#include "mdlManager.h"
#include "model.h"
#include "sdk_libc.h"
#include "sdfMemory.h"
#include "mapData.h"
#include "field.h"

/* NON_MATCHING: 85 of 366 words match. Register allocation and prologue order differ; the loop and field stores follow the asm. */
#ifdef NON_MATCHING
s32 mapObjFloorAdd_Cash(s32 floor, s16 uniq, MODELTYPE major, s32 minor, s32 motion, u32 cut_bit, s32 cash, f32 x, f32 y, f32 z, f32 r, f32 s)
{
    sdfVector4_t rot;
    sdfVector4Unaligned_t axis;
    sdfVector4Unaligned_t scl;
    sdfVector4_t pos;
    char name[32];
    sdfMemInfo_t info;
    s32 i;
    MAP_OBJ_t *o;
    dds3Basic_t *basic;
    mdlObj_t *model;
    sdfMotionCtrlr_t *ctrlr;
    fileRequest_t *pac;
    sdfPacDecodeResrc_t *res;
    sdfMemHandle_t *handle;
    s32 n;

    memset(&rot, 0, sizeof(rot));
    rot.w = 1.0f;
    axis = mapObjFloorAdd_CashTable;
    scl = mapObjFloorAdd_CashTable2;
    memset(&pos, 0, sizeof(pos));
    pos.w = 1.0f;
    mdlCallMemGetInfo();
    if (cash != 0)
    {
        sdfMemGetInfo(&info);
        if (info.maxFreeSize > 0x37FFFF)
        {
            if (info.freeSize <= 0x3FFFFF)
            {
                cash = 0;
            }
        }
        else
        {
            cash = 0;
        }
    }
    for (i = 0, o = mapObjTaskData2; o->use != 0; o++)
    {
        if (++i >= 128)
        {
            return -1;
        }
    }
    o->npc_cng = -1;
    o->evehitdir = 1;
    o->evehitsize = 150;
    o->motion_pause_spd = 0.5f;
    o->uniq = uniq;
    o->cut_bit = cut_bit;
    o->floor = floor;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    o->use = 1;
    o->visible = 1;
    o->npc_idx = -1;
    o->x = x;
    o->y = y;
    o->z = z;
    o->r = r;
    o->s = s;
    o->spcial = 0;
    o->hitsize = 0;
    o->noclip = 0;
    o->model_id1 = 0;
    o->model_id2 = 0;
    o->npc_kuse_wait = 0;
    o->npc_kuse_time = 0;
    o->user_param[0] = 0;
    o->user_param[1] = 0;
    o->user_param[2] = 0;
    o->user_param[3] = 0;
    o->motion_pause_flag = 0;
    scl.x = s;
    scl.y = s;
    scl.z = s;
    SDF_VU0_LOADVECTOR(&axis);
    FromRotation(r * 3.14f / 180.0f);
    SDF_VU0_STOREVECTOR(&rot);
    if (major == 0)
    {
        o->basic = NULL;
        return i;
    }
    basic = dds3AddUnitObjectBasic(dds3AllocObjectId(), &pos, &rot);
    o->basic = basic;
    dds3SetUnitObjectId(basic, "MAP_OBJ");
    if ((u32)(major - 1) < 3 && mdlSearchResrc(major, minor) == NULL)
    {
        handle = mdlGetModelName(major, minor);
        if (handle != NULL)
        {
            mdlPackDecode(major, minor, 0x101, sdfMemGetPtr(handle), sdfMemGetSize(handle));
        }
        else
        {
            switch (major)
            {
            case 1:
                sprintf(name, "/model/devil_lb/DEVIL%03X.LB", minor);
                break;
            case 2:
                sprintf(name, "/model/field_lb/FIELD%03X.LB", minor);
                break;
            case 3:
                sprintf(name, "/model/npc_lb/EVE%03X.LB", minor);
                break;
            }
            pac = pacLoad(name);
            pacSync(pac);
            n = 0;
            for (res = pac->pac.resrcHead; res != NULL; res = res->next, n++)
            {
                handle = res->resrc;
                if (n == 0)
                {
                    mdlPackDecode(major, minor, 0x101, sdfMemGetPtr(handle), sdfMemGetSize(handle));
                    if (cash == 0 || mdlCallMemGetSize(handle, major, minor) == 0)
                    {
                        sdfMemFreeDirect(handle);
                    }
                }
                else
                {
                    sdfMemFreeDirect(handle);
                }
            }
            pacFileHandleFree_(pac);
        }
    }
    dds3ObjectBaseMakeUnit(basic, major, minor);
    o->model_id1 = major;
    o->model_id2 = minor;
    dds3SetPosPoseSet(basic, &pos);
    dds3SetRotPoseSet(basic, &rot);
    dds3SetSclPoseSet(basic, &scl);
    dds3GetRadiusPoseSet(basic, s * 40.0f);
    dds3ObjectBaseOffFlag(basic, 0x400);
    model = dds3ObjectBaseGetModelHandle(basic);
    if (motion != -1)
    {
        mdlLoopMotion(model, 0, (u8)motion);
        model->first->speed = 0.5f;
        if ((motion & 0x100) != 0)
        {
            ctrlr = mdlGetMotionCtrlr(model, 0);
            sdfMotionSeek(ctrlr, (f32)dds3GetRandIntA(NULL, (s32)(f32)model->first->frameLength));
        }
    }
    if (major == 2 && minor == 29)
    {
        nbMoveFormation(basic);
    }
    else
    {
        mapUnitSetLight(basic, mapGetLight(pos.x, pos.z), 0);
    }
    return i;
}
#else
INCLUDE_RODATA(const s32, "code_r1_00143B78", mapObjFloorAdd_CashTable);

INCLUDE_RODATA(const s32, "code_r1_00143B78", mapObjFloorAdd_CashTable2);

INCLUDE_ASM(const s32, "code_r1_00143B78", mapObjFloorAdd_Cash);
#endif

INCLUDE_SDATA(const s32, "code_r1_00143B78", mapObjFloorAdd_CashData);

