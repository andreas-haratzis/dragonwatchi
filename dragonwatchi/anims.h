#ifndef ANIMS_H
#define ANIMS_H

#include "sprites.h"
#include <FS.h>

extern ILI9163C_color_18_t screenBuffer[128*160];

using Files = std::vector<File>;

struct Anim {
  static const size_t k_invalidLoopIdx = -1;
  
  Files sprites;
  size_t idx = 0;
  unsigned long lastDrawMillis;
  const size_t loopIdx;
  size_t loops = 0;
  
  Anim(Files&& sprites, size_t loopIdx = 0);
  void Draw(size_t x = 0, size_t y = 0);

  void Reset();
};

#endif
