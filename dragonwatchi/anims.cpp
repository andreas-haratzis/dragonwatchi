#include "anims.h"
#include "utils.h"

Anim::Anim(SpriteVec&& sprites, size_t loopIdx)
  : idx{0}
  , lastDrawMillis{millis()}
  , sprites{std::move(sprites)}
  , loopIdx(loopIdx) {
}

void Anim::Draw() {
  const unsigned long currMillis = millis();
  if(currMillis - lastDrawMillis > 1000) {
    idx = (idx + 1) % sprites.size();
    if(idx == 0) {
      idx = loopIdx;
    }
    lastDrawMillis = currMillis;
  }
  
  const ILI9163C_color_18_t* srcPtr = sprites[idx].get().pixels;
  
  for(size_t y = 0; y < 160; ++y) {
    for(size_t x = 0; x < 128; ++x) {
      const size_t i = y * 128 + x;
      const ILI9163C_color_18_t& pixel = srcPtr[i];
      if(pixel != k_magenta) {
        memcpy_P(screenBuffer + i, srcPtr + i, sizeof(ILI9163C_color_18_t));
      }
    }
  }
  
}

void Anim::Reset() {
  idx = 0;
  lastDrawMillis = millis();
}

DyingAnim::DyingAnim()
  : Anim({sprite_dying0000, sprite_dying0060}) {
}

DeadAnim::DeadAnim()
  : Anim({sprite_dead0000}) {
}

VibingHungerAnim::VibingHungerAnim()
  : Anim({sprite_vibing_hunger0000, sprite_vibing_hunger0060}) {
}

VibingHappyAnim::VibingHappyAnim()
  : Anim({sprite_vibing_happy0000}) {
}

VibingNeutralAnim::VibingNeutralAnim()
  : Anim({sprite_vibing_neutral0000}) {
}

VibingSadAnim::VibingSadAnim()
  : Anim({sprite_vibing_dying0000}) {
}

VibingDyingAnim::VibingDyingAnim()
  : Anim({sprite_vibing_dying0000}) {
}

SleepingAnim::SleepingAnim() 
  : Anim({sprite_sleeping0000, sprite_sleeping0060, sprite_sleeping0120}, 1) {
}
  
WokeAnim::WokeAnim()
  : Anim({sprite_woke0000}) {
}
