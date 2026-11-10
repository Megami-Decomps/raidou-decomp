#ifndef DDS_FILE_REQUEST_API_H

#define DDS_FILE_REQUEST_API_H
#include "common.h"

#include "file_request_cleanup_result_type.h"

struct FileRequest;

struct FilePacRequest;

struct sdfMemHandle_t;

/* Each file/PAC request copies requestName into storage it owns until cleanup.
 * The caller keeps ownership of its input string and may modify or release it
 * as soon as the call returns. */

#ifdef VERSION_DDS1

#else

#endif

/* fileLoadEx stores userData without copying it. When callbackAddress is
 * nonzero, the manager calls it once after the device request closes, passing
 * the completed request and userData. The context must remain valid until then;
 * callback delivery does not free the request wrapper, which callers release
 * with fileHandleFree. */

#ifdef VERSION_DDS1

#else

#endif

#endif /* DDS_FILE_REQUEST_API_H */
