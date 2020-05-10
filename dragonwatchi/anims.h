#ifndef ANIMS_H
#define ANIMS_H

#include "sprites.h"
#include <functional>

using SpriteVec = std::vector<std::reference_wrapper<const Sprite>>;

struct Anim {
  const SpriteVec sprites;
  size_t idx;
  unsigned long lastDrawMillis;
  size_t loopIdx;
  
  Anim(SpriteVec&& sprites, size_t loopIdx = 0);
  void Draw();
};

struct SleepingAnim : public Anim {
  SleepingAnim();
};

struct WokeAnim : public Anim {
  WokeAnim();
};

#endif
