#include "shared/effect/effModelTrackPoly/decls.h"

/* Resample the newest history span: cubic interpolation for established
 * tracks, linear extension while the history is still short. */
void dds3HermitianTrackPolySetVertex(dds3HermitianTrackPoly_tag *hPoly, const u128 *pVertex) {
    sdfVector4_t firstHistory[4];
    sdfVector4_t secondHistory[4];
    sdfVector4_t firstBasePoint;
    sdfVector4_t secondBasePoint;
    sdfVector4_t firstPointDelta;
    sdfVector4_t secondPointDelta;
    sdfVector4_t samples[2];
    s32 activePointCount = hPoly->count;
    s32 sampleCount = hPoly->split;
    f32 sampleWeightStep;
    f32 sampleWeight;
    s32 sampleIndex;
    u128 *secondSampleOutput;

    if (activePointCount == 0) {
        dds3HermitianTrackPolySetVertexSub(hPoly, pVertex);
        return;
    }

    if (activePointCount >= sampleCount * 6) {
        s32 savedRingPosition = hPoly->current;
        u128 *firstSampleOutputBase;
        u128 *firstSampleOutput;

        dds3HermitianTrackPolyGetVertex(hPoly, (u128 *)&firstHistory[0], (u128 *)&secondHistory[0], 3);
        dds3HermitianTrackPolyGetVertex(hPoly, (u128 *)&firstHistory[1], (u128 *)&secondHistory[1], 2);
        dds3HermitianTrackPolyGetVertex(hPoly, (u128 *)&firstHistory[2], (u128 *)&secondHistory[2], 1);
        SDF_VU0_COPYVECTOR(&firstHistory[3], pVertex);
        SDF_VU0_COPYVECTOR(&secondHistory[3], pVertex + 1);
        sampleWeight = 0.0f;
        sampleWeightStep = 1.0f / (f32)sampleCount;
        dds3HermitianTrackPolySetCurrent(hPoly, 2);
        if (sampleCount > 0) {
            firstSampleOutput = (u128 *)&samples[0];
            secondSampleOutput = (u128 *)&samples[1];
            firstSampleOutputBase = firstSampleOutput;
            sampleIndex = sampleCount;
            do {
                sampleWeight += sampleWeightStep;
                Eff_HermitianSpline(firstHistory, sampleWeight);
                SDF_VU0_STOREVECTOR_NOMEM(firstSampleOutput);
                Eff_HermitianSpline(secondHistory, sampleWeight);
                SDF_VU0_STOREVECTOR_NOMEM(secondSampleOutput);
                firstSampleOutput = firstSampleOutputBase;
                sampleIndex--;
                dds3HermitianTrackPolySetVertexSub(hPoly, firstSampleOutput);
            } while (sampleIndex != 0);
        }
        hPoly->count = activePointCount;
        hPoly->current = savedRingPosition;
    }

    if (activePointCount >= sampleCount * 4) {
        u128 *firstSampleOutputBase;
        u128 *firstSampleOutput;

        dds3HermitianTrackPolyGetVertex(hPoly, (u128 *)&firstHistory[0], (u128 *)&secondHistory[0], 2);
        dds3HermitianTrackPolyGetVertex(hPoly, (u128 *)&firstHistory[1], (u128 *)&secondHistory[1], 1);
        SDF_VU0_COPYVECTOR(&firstHistory[2], pVertex);
        SDF_VU0_COPYVECTOR(&secondHistory[2], pVertex + 1);
        SDF_VU0_COPYVECTOR(&firstHistory[3], pVertex);
        SDF_VU0_COPYVECTOR(&secondHistory[3], pVertex + 1);
        sampleWeight = 0.0f;
        sampleWeightStep = 1.0f / (f32)sampleCount;
        sampleIndex = 0;
        if (sampleCount > 0) {
            firstSampleOutput = (u128 *)&samples[0];
            secondSampleOutput = (u128 *)&samples[1];
            firstSampleOutputBase = firstSampleOutput;
            do {
                sampleWeight += sampleWeightStep;
                Eff_HermitianSpline(firstHistory, sampleWeight);
                SDF_VU0_STOREVECTOR_NOMEM(firstSampleOutput);
                Eff_HermitianSpline(secondHistory, sampleWeight);
                SDF_VU0_STOREVECTOR_NOMEM(secondSampleOutput);
                firstSampleOutput = firstSampleOutputBase;
                dds3HermitianTrackPolySetVertexSub(hPoly, firstSampleOutput);
                sampleIndex++;
            } while (sampleIndex < sampleCount);
        }
    } else {
        u128 *firstSampleOutput;

        dds3HermitianTrackPolyGetVertex(hPoly, (u128 *)&firstBasePoint, (u128 *)&secondBasePoint, 1);
        SDF_VU0_LOADVECTOR(pVertex);
        SDF_VU0_LOADVECTOR_B(&firstBasePoint);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_STOREVECTOR(&firstPointDelta);
        SDF_VU0_LOADVECTOR(pVertex + 1);
        SDF_VU0_LOADVECTOR_B(&secondBasePoint);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_STOREVECTOR(&secondPointDelta);
        sampleWeight = 0.0f;
        sampleWeightStep = 1.0f / (f32)sampleCount;
        if (sampleCount > 0) {
            sampleIndex = sampleCount;
            firstSampleOutput = (u128 *)&samples[0];
            do {
                sampleWeight += sampleWeightStep;
                sampleIndex--;
                samples[0].x = firstBasePoint.x + firstPointDelta.x * sampleWeight;
                samples[0].y = firstBasePoint.y + firstPointDelta.y * sampleWeight;
                samples[0].z = firstBasePoint.z + firstPointDelta.z * sampleWeight;
                samples[1].x = secondBasePoint.x + secondPointDelta.x * sampleWeight;
                samples[1].y = secondBasePoint.y + secondPointDelta.y * sampleWeight;
                samples[1].z = secondBasePoint.z + secondPointDelta.z * sampleWeight;
                dds3HermitianTrackPolySetVertexSub(hPoly, firstSampleOutput);
            } while (sampleIndex != 0);
        }
    }
}
