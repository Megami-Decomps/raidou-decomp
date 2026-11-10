#include "shared/basic/dds3SceneBasic/decls.h"

void dds3SceneBasicEndScript(dds3Basic_t *scene, const char *procname)
{
    dds3ProcessID_t *process;

    if (procname != NULL) {
        process = dds3SearchProcessName(procname);
        if (process == NULL) {
            return;
        }
        dds3KillProcess(process, 0);
    }
}
