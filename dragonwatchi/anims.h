#ifndef ANIMS_H
#define ANIMS_H

#include "sprites.h"
#include <FS.h>

extern ILI9163C_color_18_t screenBuffer[128*160];

using Files = std::vector<File>;

struct Anim {
  Files sprites;
  size_t idx;
  unsigned long lastDrawMillis;
  const size_t loopIdx;

  static const size_t k_invalidLoopIdx = -1;
  Anim(Files&& sprites, size_t loopIdx = 0);
  void Draw();

  void Reset();
};

#endif
