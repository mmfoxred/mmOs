#include "../include/asm/system.h"
#include "../include/string.h"

char *strcpy(char *dest, const char *src)
{
    char *ptr = dest;
    while (true)
    {
        *ptr++ = *src;
        if (*src++ == EOS)
            return dest;
    }
}

char *strcat(char *dest, const char *src)
{
    char *ptr = dest;
    while (*ptr != EOS)
    {
        ptr++;
    }
    while (true)
    {
        *ptr++ = *src;
        if (*src++ == EOS)
        {
            return dest;
        }
    }
}

size_t strlen(const char *str)
{
    char *ptr = (char *)str;
    while (*ptr != EOS)
    {
        ptr++;
    }
    return ptr - str;
}

int strcmp(const char *lhs, const char *rhs)
{
    while (*lhs == *rhs && *lhs != EOS && *rhs != EOS)
    {
        lhs++;
        rhs++;
    }
    return *lhs < *rhs ? -1 : *lhs > *rhs;
}

char *strchr(const char *str, int ch)
{
    char *ptr = (char *)str;
    while (true)
    {
        if (*ptr == ch)
        {
            return ptr;
        }
        if (*ptr++ == EOS)
        {
            return NULL;
        }
    }
}

char *strrchr(const char *str, int ch)
{
    char *last = NULL;
    char *ptr = (char *)str;
    while (true)
    {
        if (*ptr == ch)
        {
            last = ptr;
        }
        if (*ptr++ == EOS)
        {
            return last;
        }
    }
}

int memcmp(const void *lhs, const void *rhs, size_t count)
{
    char *lptr = (char *)lhs;
    char *rptr = (char *)rhs;
    while (*lptr == *rptr && count-- > 0)
    {
        lptr++;
        rptr++;
    }
    return *lptr < *rptr ? -1 : *lptr > *rptr;
}

void *memset(void *dest, int ch, size_t count)
{
    char *ptr = dest;
    while (count--)
    {
        *ptr++ = ch;
    }
    return dest;
}

void *memcpy(void *dst, const void *src, size_t n) {
    if (dst == NULL || src == NULL || n <= 0)
        return NULL;

    int *pdst = (int *) dst;
    int *psrc = (int *) src;
    char *tmp1 = NULL;
    char *tmp2 = NULL;
    int c1 = n / 4;
    int c2 = n % 4;

    /*if (pdst > psrc && pdst < psrc + n) 这样判断有问题*/
    if (pdst > psrc && pdst <  (psrc + n)) {
        tmp1 = (char *) pdst + n - 1;
        tmp2 = (char *) psrc + n - 1;
        while (c2--)
            *tmp1-- = *tmp2--;
        /*这样有问题，忘记字节偏移
        pdst = (int *)tmp1;
        psrc = (int *)tmp2;
        */
        tmp1++;
        tmp2++;
        pdst = (int *) tmp1;
        psrc = (int *) tmp2;
        pdst--;
        psrc--;
        while (c1--)
            *pdst-- = *psrc--;
    } else {
        while (c1--)
            *pdst++ = *psrc++;
        tmp1 = (char *) pdst;
        tmp2 = (char *) psrc;
        while (c2--)
            *tmp1++ = *tmp2++;
    }
    return dst;
}

void *memchr(const void *str, int ch, size_t count)
{
    char *ptr = (char *)str;
    while (count--)
    {
        if (*ptr == ch)
        {
            return (void *)ptr;
        }
        ptr++;
    }
}