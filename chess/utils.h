#pragma once

#include <stdbool.h>

void clear();

void substr(char *to, const char *from, unsigned char chars);

bool contains(char **pos, const char *elem);