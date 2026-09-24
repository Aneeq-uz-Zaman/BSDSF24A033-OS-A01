#include "../include/mystrfunctions.h"
#include <stddef.h>

int mystrlen(const char* s) {
    if (s == NULL) return -1;

    int len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

int mystrcpy(char* dest, const char* src) {
    if (dest == NULL || src == NULL) return -1;

    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return i;
}

int mystrncpy(char* dest, const char* src, int n) {
    if (dest == NULL || src == NULL || n < 0) return -1;

    int i = 0;
    while (i < n && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }

    int copied = i;
    while (i < n) {
        dest[i] = '\0';
        i++;
    }
    return copied;
}

int mystrcat(char* dest, const char* src) {
    if (dest == NULL || src == NULL) return -1;

    int destLen = mystrlen(dest);
    int i = 0;
    while (src[i] != '\0') {
        dest[destLen + i] = src[i];
        i++;
    }
    dest[destLen + i] = '\0';
    return destLen + i;
}
