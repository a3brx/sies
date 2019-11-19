#pragma once
#include "board.h"

struct move{
  struct piece piece;
  char initial[2];
  char final[2];
};

void play();
