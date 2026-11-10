#include "shared/effect/effPCPBoss/decls.h"

/* Same native algorithm as dds-decomp/redthing1's effPCPCrackDraw: transform
 * each trail sample, then emit and fade its delayed flash cells. */
void effPCPCrackDraw(effPCPCrack_t *work) {
    sdfVector4_t perpendicular;
    sdfVector4_t direction;
    sdfVector4_t axis;
    sdfVector4_t scaleVector;
    sdfVector4_t widthVector;
    sdfVector4_t displacement;
    sdfVector4_t baseVector;
    sdfVector4_t tipVector;
    mdlObj_t *model;
    struct sdfModelRootNode_t *map;
    s32 i;
    s32 colorStart;
    s32 mode;
    u32 tint;
    u32 center;
    u128 *historyStart;
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    s32 ramp;
    s32 colorFrames;
    u8 cellCountFromFrame;
    effPCPCrackPart_t *group;
    effPCPCrackPartFlash_t *cell;
    parCell_t *historyCell;
    u128 *positions; /* Position workspace reused for trail history and cell vertices. */
    u32 *colorWords;
    s32 frame;
    s32 groupCount;
    s32 colorCellCount;
    s32 j, count;
    s32 historyCount;
    s32 age, currentAge;
    u32 edge, alphaColor;
    f32 blend, trailLength, cellLength, taperStep;
    f32 taperRange;
    f32 baseExtent, tipExtent;

    frame = SDF_FRAME_TICK(work->frame);
    mode = work->directionMode;
    model = dds3EffectPartGetHandle(work->part);
    map = model->inner;
    SDF_VU0_LOADVECTOR(work->position);
    mdlSetPosition(model);
    scaleVector.x = work->scale;
    scaleVector.y = work->scale;
    scaleVector.z = work->scale;
    scaleVector.w = 0.0f;
    SDF_VU0_LOADVECTOR(&scaleVector);
    mdlSetScale(model);
    mdlLink(model, D_002F69B8);
    if (work->drawTrail) {
        colorStart = work->trailColorStartFrame;
        if (frame > colorStart) {
            if (work->trailColorTransitionFrames > 0) {
                blend = (f32)(frame - colorStart) / work->trailColorTransitionFrames;
                center = effColorBlend(work->trailStartCenterColor, work->trailEndCenterColor, blend);
                edge = effColorBlend(work->trailStartEdgeColor, work->trailEndEdgeColor, blend);
            } else {
                center = work->trailEndCenterColor;
                edge = work->trailEndEdgeColor;
            }
        } else {
            center = work->trailStartCenterColor;
            edge = work->trailStartEdgeColor;
        }
        groupCount = work->MarkCount;
        for (i = 0; i < groupCount; i++) {
            colorWords = work->system->cells[i].colors;
            colorCellCount = work->cellCount;
            if (colorCellCount != 0) {
                j = colorCellCount;
                do {
                    colorWords[0] = edge;
                    colorWords[1] = center;
                    colorWords[2] = edge;
                    colorWords += 3;
                    --j;
                } while (j != 0);
            }
            groupCount = work->MarkCount;
        }
    }
    groupCount = work->MarkCount;
    widthVector.x = (work->trailWidth * 0.5f) * scaleVector.x;
    widthVector.y = (work->trailWidth * 0.5f) * scaleVector.x;
    widthVector.z = (work->trailWidth * 0.5f) * scaleVector.x;
    widthVector.w = 0.0f;
    perpendicular.x = perpendicular.y = perpendicular.z = perpendicular.w = widthVector.w;
    tint = work->abgr;
    i = 0;
    if (groupCount > 0) {
        taperRange = -1.0f;
        do {
            historyCell = &work->system->cells[i];
            positions = historyCell->history;
            historyCell->color = tint;
            if (mode == 2) {
                direction.x = 0.0f;
                direction.y = 0.0f;
                direction.z = 1.0f;
                sdfModelGetMarkPosMatrix(map, i);
                SDF_VU0_LOADVECTOR(&direction);
                SDF_VU0_APPLYMATRIX_XYZ();
                SDF_VU0_STOREVECTOR(&direction);
                work->pPart[i].dir[0] = direction.x;
                work->pPart[i].dir[1] = direction.y;
                work->pPart[i].dir[2] = direction.z;
            }
            sdfModelGetMarkPos(map, i);
            if (frame < work->cellCount) {
                historyCount = historyCell->vertexCount;
                SDF_FRAME_WHEN(work->frame, SDF_VU0_STOREVECTOR_NOMEM(positions + historyCount));
                SDF_FRAME_WHEN(work->frame, SDF_VU0_STOREVECTOR_NOMEM(positions + historyCount + 1));
                SDF_FRAME_WHEN(work->frame, SDF_VU0_STOREVECTOR_NOMEM(positions + historyCount + 2));
                historyCell->vertexCount += 3 * SDF_FRAME_DUE(work->frame);
                taperStep = taperRange / frame;
                blend = 1.0f;
                SDF_VU0_LOADVECTOR_C(D_0031FF70);
                if (frame > 0) {
                    const f32 *trailWidth = &widthVector.x;
                    j = frame;
                    do {
                        SDF_VU0_LOADVECTOR(positions + 4);
                        SDF_VU0_LOADVECTOR_B(positions + 1);
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_GETLENGTH(trailLength);
                        if (trailLength > 1.0f) {
                            SDF_VU0_COPYVECTOR_C_TO_B();
                            SDF_VU0_OUTERPRODUCT();
                            SDF_VU0_NORMALIZE();
                            SDF_VU0_STOREVECTOR(&perpendicular);
                        } else {
                            SDF_VU0_LOADVECTOR(&perpendicular);
                        }
                        SDF_VU0_LOADVECTOR_B(trailWidth);
                        SDF_VU0_SCALEVECTOR_B_BITS(blend);
                        SDF_VU0_MULVECTOR();
                        SDF_VU0_COPYVECTOR_A_TO_B();
                        SDF_VU0_LOADVECTOR(positions + 1);
                        SDF_VU0_ADDVECTOR();
                        SDF_VU0_STOREVECTOR(positions);
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_STOREVECTOR(positions + 2);
                        --j;
                        blend += taperStep;
                        positions += 3;
                    } while (j != 0);
                }
            }
            i++;
        } while (i < groupCount);
    }
    if (work->drawTrail) {
        dds3TrackPolyDraw(work->system);
    }
    if (work->groupsHandle != NULL) {
        duration = work->cellDuration;
        count = frame - work->cellStartFrame;
        fadeIn = work->cellFadeIn;
        fadeOut = work->cellFadeOut;
        colorStart = work->cellColorStartAge;
        colorFrames = work->cellColorTransitionFrames;
        cellCountFromFrame = work->cellCountFromFrame;
        if (count > 0) {
            ramp = work->offsetRampFrames;
            if (cellCountFromFrame == 1) count = frame;
            if (count > work->cellCount) count = work->cellCount;
            group = work->pPart;
            for (i = 0; i < groupCount; i++, group++) {
                historyCell = &work->system->cells[i];
                historyStart = historyCell->history;
                cell = group->pSize;
                dds3FlashPolyListSetABGR(group->hFlash, tint);
                for (j = 0; j < count - 1; j++, cell++, historyStart += 3) {
                    positions = (u128 *)dds3FlashPolyListGetVertex(group->hFlash, j);
                    colorWords = dds3FlashPolyListGetVC(group->hFlash, j)->color;
                    age = cell->frame;
                    if (age > colorStart) {
                        if (work->cellColorTransitionFrames > 0 && age < colorStart + colorFrames) {
                            blend = (f32)(age - colorStart) / colorFrames;
                            center = effColorBlend(work->cellStartCenterColor, work->cellEndCenterColor, blend);
                            edge = effColorBlend(work->cellStartEdgeColor, work->cellEndEdgeColor, blend);
                        } else {
                            center = work->cellEndCenterColor;
                            edge = work->cellEndEdgeColor;
                        }
                    } else {
                        center = work->cellStartCenterColor;
                        edge = work->cellStartEdgeColor;
                    }
                    center &= 0xFFFFFF;
                    edge &= 0xFFFFFF;
                    currentAge = cell->frame;
                    if (currentAge >= 0) {
                        if (currentAge >= ramp) direction.y = cell->length;
                        else direction.y = (cell->length * currentAge) / ramp;
                        displacement.x = group->dir[0] * direction.y;
                        displacement.y = group->dir[1] * direction.y;
                        displacement.z = group->dir[2] * direction.y;
                        displacement.w = 0.0f;
                        if (mode == 2 && currentAge >= group->rotationStartAge) {
                            SDF_VU0_LOADVECTOR(historyStart + 4);
                            SDF_VU0_LOADVECTOR_B(historyStart + 1);
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_NORMALIZE();
                            SDF_VU0_STOREVECTOR_NOMEM(&axis);
                            SetRotateMatrixAxis(group->rotate, &axis);
                            SDF_VU0_LOADVECTOR(&displacement);
                            SDF_VU0_APPLYMATRIX_XYZ();
                            SDF_VU0_STOREVECTOR(&displacement);
                            group->rotate += SDF_FRAME_STEP(group->rotate_add);
                        }
                        baseExtent = cell->width_in;
                        baseVector.x = baseExtent;
                        baseVector.y = baseExtent;
                        baseVector.z = baseExtent;
                        baseVector.w = 0.0f;
                        tipExtent = cell->width_out;
                        tipVector.x = tipExtent;
                        tipVector.y = tipExtent;
                        tipVector.z = tipExtent;
                        tipVector.w = 0.0f;
                        SDF_VU0_LOADVECTOR(historyStart + 4);
                        SDF_VU0_LOADVECTOR_B(historyStart + 1);
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_GETLENGTH(cellLength);
                        if (cellLength > 1.0f) {
                            SDF_VU0_NORMALIZE();
                            SDF_VU0_COPYVECTOR_A_TO_B();
                            SDF_VU0_LOADVECTOR(&tipVector);
                            SDF_VU0_MULVECTOR();
                            SDF_VU0_STOREVECTOR(&tipVector);
                            SDF_VU0_LOADVECTOR(&baseVector);
                            SDF_VU0_MULVECTOR();
                            SDF_VU0_STOREVECTOR(&baseVector);
                            SDF_VU0_LOADVECTOR_C(&displacement);
                            SDF_VU0_LOADVECTOR(historyStart + 4);
                            SDF_VU0_LOADVECTOR_B(&baseVector);
                            SDF_VU0_STOREVECTOR(positions + 3);
                            SDF_VU0_ADDVECTOR();
                            SDF_VU0_STOREVECTOR(positions + 2);
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_STOREVECTOR(positions + 4);
                            SDF_VU0_LOADVECTOR(historyStart + 4);
                            SDF_VU0_LOADVECTOR_B(&tipVector);
                            SDF_VU0_ADDVECTOR_A_C();
                            SDF_VU0_ADDVECTOR();
                            SDF_VU0_STOREVECTOR(positions);
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_STOREVECTOR(positions + 1);
                        } else {
                            SDF_VU0_LOADIDENTITYVECTOR();
                            SDF_VU0_STOREVECTOR(positions);
                            SDF_VU0_STOREVECTOR(positions + 1);
                            SDF_VU0_STOREVECTOR(positions + 2);
                            SDF_VU0_STOREVECTOR(positions + 3);
                            SDF_VU0_STOREVECTOR(positions + 4);
                        }
                        alphaColor = 0;
                        if (age <= duration) {
                            if (age < fadeIn && fadeIn != 0) {
                                blend = (f32)age / fadeIn;
                                alphaColor = effMagaCylinderCalcColor(((u32)(blend * 2147483648.0f) & 0xFF000000) | 0x808080, tint);
                            } else if (duration - age <= fadeOut && fadeOut != 0) {
                                blend = (f32)(duration - age) / fadeOut;
                                alphaColor = effMagaCylinderCalcColor(((u32)(blend * 2147483648.0f) & 0xFF000000) | 0x808080, tint);
                            } else alphaColor = tint;
                        }
                        colorWords[0] = effMagaCylinderCalcColor(edge, alphaColor);
                        colorWords[1] = effMagaCylinderCalcColor(edge, alphaColor);
                        if (cell->type) {
                            colorWords[2] = effMagaCylinderCalcColor(0x80000000, alphaColor);
                            colorWords[3] = effMagaCylinderCalcColor(center | 0xFF000000, alphaColor);
                            colorWords[4] = colorWords[2];
                        } else {
                            colorWords[2] = effMagaCylinderCalcColor(0xFF000000, alphaColor);
                            colorWords[3] = effMagaCylinderCalcColor(center | 0x40000000, alphaColor);
                            colorWords[4] = colorWords[2];
                        }
                    }
                    cell->frame += SDF_FRAME_DUE(work->frame);
                }
                dds3FlashPolyListDraw(group->hFlash);
            }
        }
    }
    work->frame++;
}
