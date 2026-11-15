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

#endif
