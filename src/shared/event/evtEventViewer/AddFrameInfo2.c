#include "shared/event/evtEventViewer/decls.h"

evtFrameInfo_t *AddFrameInfo2(evtViewerObject_s *group, s32 frame, evtViewerInfo_t *viewer)
{
    return evtAddFrameInfo(group, frame);
}
