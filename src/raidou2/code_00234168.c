#include "common.h"

INCLUDE_ASM(const s32, "code_00234168", fileJobFreeSecondaryBuffer);

INCLUDE_ASM(const s32, "code_00234168", fileJobCreateChild);

INCLUDE_ASM(const s32, "code_00234168", fileJobNotifyPair);

INCLUDE_ASM(const s32, "code_00234168", fileJobNotifyComplete);

INCLUDE_ASM(const s32, "code_00234168", fileJobInvokeTypeCallback);

INCLUDE_ASM(const s32, "code_00234168", fileJobInvokePositionCallback);

INCLUDE_ASM(const s32, "code_00234168", fileJobInvokeRotationCallback);

INCLUDE_ASM(const s32, "code_00234168", fileJobInvokeScaleCallback);

INCLUDE_ASM(const s32, "code_00234168", fileDispatchJobTypeCallback);

INCLUDE_ASM(const s32, "code_00234168", fileJobSetPrimaryData);

INCLUDE_ASM(const s32, "code_00234168", fileJobCopyCommandIntoPrimaryData);

INCLUDE_ASM(const s32, "code_00234168", fileJobSetSecondaryData);

INCLUDE_ASM(const s32, "code_00234168", fileJobCopyCommandIntoSecondaryData);

INCLUDE_ASM(const s32, "code_00234168", fileJobWriteSerializedPayload);

INCLUDE_ASM(const s32, "code_00234168", fileWriteToPfs);

INCLUDE_ASM(const s32, "code_00234168", fileDuplicateJob);

INCLUDE_ASM(const s32, "code_00234168", fileJobCreateFromCommandState);

INCLUDE_ASM(const s32, "code_00234168", fileJobSerializedSize);

INCLUDE_ASM(const s32, "code_00234168", func_00234AA0);

INCLUDE_ASM(const s32, "code_00234168", func_00234AF8);

INCLUDE_ASM(const s32, "code_00234168", fileQueueAppend);

INCLUDE_ASM(const s32, "code_00234168", fileQueueInsertAfter);

INCLUDE_ASM(const s32, "code_00234168", fileQueueRemove);

INCLUDE_ASM(const s32, "code_00234168", func_00234BF0);

INCLUDE_ASM(const s32, "code_00234168", func_00234C28);

INCLUDE_ASM(const s32, "code_00234168", fileDestroyJob);

INCLUDE_ASM(const s32, "code_00234168", fileCloneQueueEntries);

INCLUDE_ASM(const s32, "code_00234168", CallShopTest);

INCLUDE_ASM(const s32, "code_00234168", func_00234ED8);

INCLUDE_ASM(const s32, "code_00234168", fileQueueDestroy);

INCLUDE_ASM(const s32, "code_00234168", fileQueueClone);

INCLUDE_ASM(const s32, "code_00234168", fileQueueNotifyAllJobsComplete);

INCLUDE_ASM(const s32, "code_00234168", fileQueueSetPosition);

INCLUDE_ASM(const s32, "code_00234168", fileQueueSetRotation);

INCLUDE_ASM(const s32, "code_00234168", fileQueueSetScale);

INCLUDE_ASM(const s32, "code_00234168", fileQueueSetColor);

INCLUDE_ASM(const s32, "code_00234168", fileQueueCopyRotationFromSource);

INCLUDE_ASM(const s32, "code_00234168", fileAppendJob);

INCLUDE_ASM(const s32, "code_00234168", fileDuplicateAndAppendJob);

INCLUDE_ASM(const s32, "code_00234168", fileAppendJobFromCommandPath);

INCLUDE_ASM(const s32, "code_00234168", fileJobDuplicateAfter);

INCLUDE_ASM(const s32, "code_00234168", fileQueueLinkJobToSectorLeader);

INCLUDE_ASM(const s32, "code_00234168", fileQueueDetachSectorFollower);

INCLUDE_ASM(const s32, "code_00234168", fileQueueRemoveAndDestroyJob);

INCLUDE_ASM(const s32, "code_00234168", func_00235B08);

INCLUDE_ASM(const s32, "code_00234168", fileQueueSaveImage);

INCLUDE_ASM(const s32, "code_00234168", fileQueueSaveVersionedImage);

INCLUDE_ASM(const s32, "code_00234168", fileQueueCreateFromCommandState);

INCLUDE_ASM(const s32, "code_00234168", fileQueueFindById);

INCLUDE_ASM(const s32, "code_00234168", fileQueueFindFlaggedById);

INCLUDE_ASM(const s32, "code_00234168", fileQueueFindBySector);

INCLUDE_ASM(const s32, "code_00234168", fileQueueGetAt);

INCLUDE_ASM(const s32, "code_00234168", fileFindQueuedJobIndex);

INCLUDE_ASM(const s32, "code_00234168", fileQueueCountLinkedJobs);

INCLUDE_ASM(const s32, "code_00234168", camFollowOffsetVec);

INCLUDE_ASM(const s32, "code_00234168", camAimRotation);

INCLUDE_ASM(const s32, "code_00234168", fileCreateSurfaceLoaderState);

INCLUDE_ASM(const s32, "code_00234168", fileCreateGridLoaderRecord);

INCLUDE_ASM(const s32, "code_00234168", func_00236B50);

INCLUDE_ASM(const s32, "code_00234168", fileDestroyEffectSurfaceAndChildren);

INCLUDE_ASM(const s32, "code_00234168", fileLoadObjectCreateChild);

INCLUDE_ASM(const s32, "code_00234168", func_00236DD0);

INCLUDE_ASM(const s32, "code_00234168", fileLoadObjectSetResource);

INCLUDE_ASM(const s32, "code_00234168", fileLoadObjectOpenNamedDevice);

INCLUDE_ASM(const s32, "code_00234168", fileLoadObjectOpenDevice);

INCLUDE_ASM(const s32, "code_00234168", fileLoadObjectOpenAndStartDevice);

INCLUDE_ASM(const s32, "code_00234168", fileReplaceEffectSurfaceJobs);

INCLUDE_ASM(const s32, "code_00234168", fileReplaceEffectSurfaceQueues);

INCLUDE_ASM(const s32, "code_00234168", fileReplaceReferenceHolder);

INCLUDE_ASM(const s32, "code_00234168", fileClearLoadObjectReferences);

INCLUDE_ASM(const s32, "code_00234168", func_00237478);

INCLUDE_ASM(const s32, "code_00234168", func_002374A8);

INCLUDE_ASM(const s32, "code_00234168", func_00237E48);

INCLUDE_ASM(const s32, "code_00234168", fileSendLoadObjectRecordVector);

INCLUDE_ASM(const s32, "code_00234168", fileCopyLoadObjectRecordVector);

INCLUDE_ASM(const s32, "code_00234168", fileSetRecordWordFour);

INCLUDE_ASM(const s32, "code_00234168", fileSetLoadObjectScale);

INCLUDE_ASM(const s32, "code_00234168", fileResetSlotStates);

INCLUDE_ASM(const s32, "code_00234168", effSampleColorAlphaTracks);

INCLUDE_ASM(const s32, "code_00234168", effSampleScalarCurve);

INCLUDE_ASM(const s32, "code_00234168", func_002383B0);

INCLUDE_ASM(const s32, "code_00234168", func_00238520);

INCLUDE_ASM(const s32, "code_00234168", fileInvalidateSlotGroup);

INCLUDE_ASM(const s32, "code_00234168", fileCopyAndInvalidateSlotGroup);

INCLUDE_ASM(const s32, "code_00234168", effUpdateGridRecordSlotHistory);

INCLUDE_ASM(const s32, "code_00234168", effUpdateWaveEmitterSlots);

INCLUDE_ASM(const s32, "code_00234168", effScaleWaveEmitterParameters);

INCLUDE_ASM(const s32, "code_00234168", effUpdateRadialEmitterSlots);

INCLUDE_ASM(const s32, "code_00234168", func_00239DD8);

INCLUDE_ASM(const s32, "code_00234168", effUpdateCircularEmitterSlots);

INCLUDE_ASM(const s32, "code_00234168", func_0023A698);

INCLUDE_ASM(const s32, "code_00234168", effUpdateOrientedRingEmitterSlots);

INCLUDE_ASM(const s32, "code_00234168", func_0023AF10);

INCLUDE_ASM(const s32, "code_00234168", effUpdateDirectedEmitterSlots);

INCLUDE_ASM(const s32, "code_00234168", func_0023B690);

INCLUDE_ASM(const s32, "code_00234168", effUpdateSectorEmitterSlots);

INCLUDE_ASM(const s32, "code_00234168", func_0023BE38);

INCLUDE_ASM(const s32, "code_00234168", func_0023BEC0);
INCLUDE_SDATA(const s32, "code_00234168", D_004E1820);

INCLUDE_SDATA(const s32, "code_00234168", D_004E1828);

INCLUDE_SDATA(const s32, "code_00234168", D_004E1830);

INCLUDE_SDATA(const s32, "code_00234168", D_004E1838);

