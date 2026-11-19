#include "common.h"
#include "kernel.h"
#include "sdfMemory.h"
#include "fldNme.h"
#include "dds3KernelCore.h"
#include "dds3KernelMain.h"
#include "frFont.h"
#include "dds3PadManager.h"
#include "sdfSound.h"

s32 fldCallMemGetPtr(dds3ProcessID_t *id)
{
    fldNmeWork_t *work = sdfMemGetPtr(id->workdata);
    struct sdfMemHandle_t *handle;
    fldNmeSlotList_t *list;
    s32 cmd;
    s32 pos;
    s32 cur;
    s32 index;
    s32 page;
    s32 sel;

    switch (work->unk8) {
    case 0:
        if (pacFileCheckEnd(work->font)) {
            handle = fileGetMemHandle(work->font);
            frCallMemLock(4, sdfMemGetPtr(handle), handle);
            fileHandleFree(work->font);
            fldCallMemGetPtr7(id);
            fclFunc3(&work->draw);
            work->unk8 = 1;
        }
        break;
    case 1:
        cmd = 0;
        if ((work->unk0 & 2) && dds3PadButtonState[0x26] == 0 && dds3PadButtonState[0x27] == 0) {
            work->unk0 &= ~2;
        }
        if ((work->unk0 & 1) && dds3PadButtonState[0x24] == 0 && dds3PadButtonState[0x25] == 0) {
            work->unk0 &= ~1;
        }
        if (dds3PadButtonState[0x2C] < 0) {
            cmd = 18;
            p2sdPlaySE(0, 127, 63);
        } else if (dds3PadButtonState[0x23] < 0) {
            switch (work->unk10) {
            case 0:
                cmd = 1;
                p2sdPlaySE(0, 127, 63);
                break;
            case 1:
                switch (work->unk3C) {
                case 0:
                    cmd = 17;
                    break;
                case 1:
                    cmd = 16;
                    break;
                case 2:
                    cmd = 2;
                    break;
                case 3:
                    cmd = 21;
                    break;
                }
                p2sdPlaySE(0, 127, 63);
                break;
            case 2:
                break;
            }
        } else if (dds3PadButtonState[0x21] < 0) {
            cmd = 2;
            p2sdPlaySE(1, 127, 63);
        } else if ((((dds3PadButtonState[0x26] & 2) || (dds3PadButtonState[0x27] & 2)) && (~work->unk0 & 2))
                   || (((dds3PadButtonState[0x24] & 2) || (dds3PadButtonState[0x25] & 2)) && (~work->unk0 & 1))) {
            p2sdPlaySE(3, 127, 63);
            switch (work->unk10) {
            case 0:
                if (dds3PadButtonState[0x26] & 2) {
                    cmd = 3;
                } else if (dds3PadButtonState[0x27] & 2) {
                    cmd = 4;
                } else if (dds3PadButtonState[0x24] & 2) {
                    if (work->unk34 == 0) {
                        cmd = (work->unkC == 3) ? 12 : 11;
                    } else {
                        cmd = 5;
                    }
                } else if (dds3PadButtonState[0x25] & 2) {
                    cmd = (work->unk34 == work->unk44 - 1) ? 11 : 6;
                }
                break;
            case 1:
                if (dds3PadButtonState[0x26] & 2) {
                    cmd = 13;
                } else if (dds3PadButtonState[0x27] & 2) {
                    cmd = 14;
                } else if (dds3PadButtonState[0x24] & 2) {
                    cmd = 10;
                } else if (dds3PadButtonState[0x25] & 2) {
                    cmd = (work->unkC == 3) ? 12 : 9;
                }
                break;
            case 2:
                if (dds3PadButtonState[0x26] & 2) {
                    cmd = 8;
                } else if (dds3PadButtonState[0x27] & 2) {
                    cmd = 7;
                } else if (dds3PadButtonState[0x24] & 2) {
                    cmd = 11;
                } else if (dds3PadButtonState[0x25] & 2) {
                    cmd = 9;
                }
                break;
            }
        } else if ((dds3PadButtonState[0x29] & 2) || (dds3PadButtonState[0x2B] & 2)) {
            if (dds3PadButtonState[0x2B] & 2) {
                if (work->unk14 != 1) {
                    p2sdPlaySE(3, 127, 63);
                }
                cmd = 20;
            } else {
                if (work->unk14 != 0) {
                    p2sdPlaySE(3, 127, 63);
                }
                cmd = 19;
            }
        }

        switch (cmd) {
        case 1:
            if (fldCallMemGetPtr6(id, &index) == 0) {
                break;
            }
            list = &work->list[work->unk14];
            fclCallSetFontString2(list, index);
            if (list->unk2 < list->count - 1) {
                list->unk2++;
            } else if (work->unk14 <= 0) {
                work->unk14++;
            }
            break;
        case 2:
            list = &work->list[work->unk14];
            if (list->slot[list->unk2].id == 0xFFFF) {
                if (list->unk2 == 0) {
                    if (work->unk14 == 0) {
                        break;
                    }
                    work->unk14--;
                } else {
                    list->unk2--;
                }
            }
            fclFunc9(list);
            break;
        case 3:
            pos = work->unk38;
            pos--;
            switch (work->unkC) {
            case 3:
                if (pos < work->unk30) {
                    if (pos < 0) {
                        pos = titleFunc3() - 1;
                        work->unk30 = pos;
                    } else {
                        work->unk30 = pos;
                    }
                    fclNakamaCashLoadDecodeSub(&work->draw, work->unkC, work->unk30);
                }
                break;
            case 0:
            case 1:
            case 2:
                if (pos < 0) {
                    pos = work->unk48 - 1;
                }
                if (pos == 0) {
                    work->unk0 |= 2;
                }
                break;
            }
            work->unk38 = pos;
            break;
        case 4:
            fldNakamaCashLoad_Decode(id);
            if (work->unkC < 3 && work->unk38 == work->unk48 - 1) {
                work->unk0 |= 2;
            }
            break;
        case 5:
            sel = work->unk34;
            sel--;
            if (sel == 0) {
                work->unk0 |= 1;
            }
            work->unk34 = sel;
            break;
        case 6:
            sel = work->unk34 + 1;
            if (work->unkC < 4 && sel >= work->unk44) {
                sel = 0;
            }
            if (sel == work->unk44 - 1) {
                work->unk0 |= 1;
            }
            work->unk34 = sel;
            break;
        case 8:
            titleFunc4(titleFunc6(work->unk38), &page, &index);
            if (index == 0) {
                page = (page - 1 >= 0) ? page - 1 : 43;
            }
            cur = titleFunc5(page);
            work->unk34 = 0;
            work->unk30 = cur;
            work->unk38 = cur;
            fclNakamaCashLoadDecodeSub(&work->draw, work->unkC, cur);
            break;
        case 7:
            titleFunc4(titleFunc6(work->unk38), &page, &index);
            page++;
            if (page == 44) {
                page = 0;
            }
            cur = titleFunc5(page);
            work->unk34 = 0;
            work->unk30 = cur;
            work->unk38 = cur;
            fclNakamaCashLoadDecodeSub(&work->draw, work->unkC, cur);
            break;
        case 9:
            work->unk34 = 0;
            work->unk10 = 0;
            break;
        case 10:
            work->unk10 = 0;
            work->unk34 = work->unk44 - 1;
            break;
        case 11:
            work->unk10 = 1;
            break;
        case 12:
            work->unk10 = 2;
            break;
        case 13:
            work->unk3C--;
            if (work->unk3C < 0) {
                work->unk3C = 3;
            }
            if (work->unk3C == 0) {
                work->unk0 |= 2;
            }
            break;
        case 14:
            work->unk3C++;
            if (work->unk3C >= 4) {
                work->unk3C = 0;
            }
            if (work->unk3C == 3) {
                work->unk0 |= 2;
            }
            break;
        case 15:
            fclFunc5(&work->draw);
            work->unk44 = titleStartSub(work->unkC);
            work->unk48 = titleStartSub2(work->unkC);
            work->unk34 = 0;
            work->unk38 = 0;
            work->unk30 = 0;
            fclCallSetSpriteNormal(&work->draw, work->unkC, 0);
            work->unk10 = 0;
            break;
        case 16:
            list = &work->list[work->unk14];
            if (list->unk2 == 0) {
                if (work->unk14 <= 0) {
                    break;
                }
                work->unk14--;
            } else {
                list->unk2--;
            }
            break;
        case 17:
            list = &work->list[work->unk14];
            if (list->unk2 == list->count - 1) {
                if (work->unk14 > 0) {
                    break;
                }
                work->unk14++;
            } else {
                list->unk2++;
            }
            break;
        case 18:
            if (fldCallMemGetPtr3(id, &index) == 0) {
                work->unk14 = index;
                break;
            }
            fclLeaf19(&work->draw);
            work->unk40 = 1;
            work->unk8 = 2;
            break;
        case 19:
            work->unk14 = 1;
            break;
        case 20:
            work->unk14 = 0;
            break;
        case 21:
            if (fldCallMemGetPtr3(id, &index) == 0) {
                break;
            }
            fclLeaf19(&work->draw);
            work->unk40 = 1;
            work->unk8 = 2;
            break;
        }
        break;
    case 2:
        if (dds3PadButtonState[0x23] < 0 && work->unk40 == 0) {
            fldCallMemGetPtr8(id);
            p2sdPlaySE(0, 127, 63);
            return -1;
        }
        if (dds3PadButtonState[0x21] < 0 || (dds3PadButtonState[0x23] < 0 && work->unk40 == 1)) {
            fclLeaf20(&work->draw);
            work->unk8 = 1;
            p2sdPlaySE(1, 127, 63);
        } else if (dds3PadButtonState[0x26] < 0) {
            work->unk40 = 0;
            p2sdPlaySE(3, 127, 63);
        } else if (dds3PadButtonState[0x27] < 0) {
            work->unk40 = 1;
            p2sdPlaySE(3, 127, 63);
        }
        break;
    }
    return 0;
}

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/002FA1E8.c:1, found */
s32 fldCallMemGetPtr2(dds3ProcessID_t *id) {
    fldNmeWork_t *work = sdfMemGetPtr(id->workdata);

    switch (work->unk8) {
    case 1:
        fclCallSetSelect(&work->draw, 0);
        fclLeaf15(&work->draw);
        fldCallMemGetPtr9(id);
        fclCallSetFontColorNo(&work->draw, work->unkC, work->unk30, work->unk10, work->unk34, work->unk38);
        fclFunc6(&work->draw, work->unk10, work->unk3C);
        fldCallMemGetPtr10(id);
        break;
    case 2:
        fclCallSetSelect(&work->draw, 0);
        fclLeaf15(&work->draw);
        fldCallMemGetPtr9(id);
        fclFunc6(&work->draw, work->unk10, work->unk3C);
        fldCallMemGetPtr11(id);
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_002F97C0", Finish_nmeMain);

void fldCallMemGetPtr5(dds3ProcessID_t *id) {
    fldNmeWork_t *work = sdfMemGetPtr(id->workdata);
    u32 i;

    for (i = 0; i < 2; i++) {
        fclFunc10(&work->list[i]);
    }
    fclFunc4(&work->draw);
    if (work->drawProcess != NULL) {
        dds3KillProcess(work->drawProcess, 0);
    }
    frCallMemFreeDirect(4);
    dds3KernelDebugSign = 1;
    sdfMemFreeDelayedDirect(id->workdata);
}

s32 fldCallMemGetPtr6(dds3ProcessID_t *id, s32 *code) {
    fldNmeWork_t *work = sdfMemGetPtr(id->workdata);
    s32 page;
    s32 index;
    u32 count;
    s32 ret;

    if (work->unkC >= 3) {
        if (work->unkC == 3) {
            titleFunc4(titleFunc6(work->unk38), &page, &index);
            index += work->unk34;
            count = fclLeaf23(page);
            if (index >= count) {
                return 0;
            }
            *code = fclLeaf24(page, index);
            return 1;
        }
    } else {
        ret = titleFunc(work->unkC, work->unk34, work->unk38);
        if (ret != 0) {
            index = nmeLeaf2(work->unkC, work->unk34, work->unk38);
            *code = index;
            return 1;
        }
        return ret;
    }
    return 0;
}
