#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "libsdf.h"

typedef struct fileRequest {
    u8 kind;
    u8 state;
    u8 retry;
    u8 mode;
    struct fileRequest *next;
    char *name;
    void *dev;
    s32 rest;
    s32 size;
    void *callback;
    void *arg;
    sdfMemHandle_t *handle;
    addr_t top;
    addr_t ptr;
    struct fileRequest *donenext;
    sdfPacDecodeCb_t pac;
    u16 pacrest;
    u16 slot;
    struct fileRequest *cbnext;
} fileRequest_t;

fileRequest_t *fileLoad(const char *filename);
fileRequest_t *fileLoadUseTail(const char *filename);

u8 *fileGetLoadPtr(const fileRequest_t *fileHandle);
u32 fileGetLoadSize(const fileRequest_t *fileHandle);
sdfMemHandle_t *fileGetMemHandle(const fileRequest_t *fileHandle);
s32 fileHandleFree(fileRequest_t *fileHandle);
fileRequest_t *fileLoadEx(const char *filename, u32 headOrTail, addr_t endCallback, addr_t callbackParm);
s32 fileManager(void);
void fileSync(fileRequest_t *fileHandle);
void fileSyncAll(void);
s32 pacCheckEnd(const fileRequest_t *pacHandle);
s32 pacFileCheckEnd(const fileRequest_t *pacFileHandle);
s32 pacFileHandleFree_(void *pacHandle);
fileRequest_t *pacLoad(const char *filename);
fileRequest_t *pacLoadEx(const char *filename, u32 headOrTail, u32 callback, u32 pendCallback, u32 callbackParm);
void pacSync(fileRequest_t *pacFileHandle);
void fileSysInit(void);

#endif
