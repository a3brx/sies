#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clear() {
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#define PLATFORM_NAME "windows"
#elif defined(__linux__)
#define PLATFORM_NAME "linux"
#endif
    if (PLATFORM_NAME == "linux")
        printf("\033[2J\033[1;1H");
    else if (PLATFORM_NAME == "windows")
        system("cls");
}

void substr(char *to, const char *from, unsigned char chars) {
    for (unsigned char index = 0; index < chars; ++index)
        *(to + index) = *(from + index);
    *(to + chars) = '\0';
}

bool contains(char **pos, const char *elem) {
    while (*pos != NULL) {
        if (strcmp(*pos, elem) == 0)
            return true;
        pos++;
    }
    return false;
}