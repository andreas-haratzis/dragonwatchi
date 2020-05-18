#ifndef ANIMS_H
#define ANIMS_H

#include "sprites.h"
#include <functional>

using SpriteVec = std::vector<std::reference_wrapper<const Sprite>>;

struct Anim {
  const SpriteVec sprites;
  size_t idx;
  unsigned long lastDrawMillis;
  const size_t loopIdx;
  
  Anim(SpriteVec&& sprites, size_t loopIdx = 0);
  void Draw();

  void Reset();
};

struct DyingAnim : public Anim {
  DyingAnim();
};

struct DeadAnim : public Anim {
  DeadAnim();
};

struct VibingHungerAnim : public Anim {
  VibingHungerAnim();
};

struct VibingHappyAnim : public Anim {
  VibingHappyAnim();
};

struct VibingNeutralAnim : public Anim {
  VibingNeutralAnim();
};

struct VibingSadAnim : public Anim {
  VibingSadAnim();
};

struct VibingDyingAnim : public Anim {
  VibingDyingAnim();
};

struct SleepingAnim : public Anim {
  SleepingAnim();
};

struct WokeAnim : public Anim {
  WokeAnim();
};

#endif
