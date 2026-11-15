#ifndef SDK_LIBC_H
#define SDK_LIBC_H

#include "common.h"

s32 printf(const char *format, ...);
s32 sprintf(char *destination, const char *format, ...);

void *memcpy(void *, const void *, size_t);

void *memset(void *a, s32 b, size_t c);

u32 strlen(const char *text);

void free(void *ptr);
void *malloc(size_t size);

char *strcpy(char *dst, const char *src);

s32 memcmp(const void *left, const void *right, size_t count);

s32 strcmp(const char *left, const char *right);

char *strcat(char *dst, const char *src);

s32 exponent(char *output, s32 value, s32 marker);

s32 fflush(void *stream);

char *strrchr(const char *s, s32 c);

s32 strcasecmp(const char *left, const char *right);

void *memmove(void *a, const void *b, size_t c);

long strtol(const char *s, char **end, s32 base);

char *strncpy(char *destination, const char *source, size_t count);

s32 strncmp(const char *, const char *, size_t);

char * strchr(const char *, s32);

long __sseek(void *cookie, s32 offset, s32 whence);

s32 raise(s32 sig);

void abort(void);

#endif
