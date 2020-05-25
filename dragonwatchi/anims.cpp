#include "anims.h"
#include "utils.h"
#include "sd.h"

ILI9163C_color_18_t screenBuffer[128*160] = {0};

Anim::Anim(Files&& sprites, size_t loopIdx)
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

  Sprite sprite = LoadSprite(sprites[idx]);

  const ILI9163C_color_18_t* srcPtr = sprite.pixels;
  
  // BMPs are horizontal but the screen is vertical
  // and flipped
   
  size_t srcX = 159;
  for(size_t dstY = 0; dstY < 160; ++dstY) {
    size_t srcY = 127;
    for(size_t dstX = 0; dstX < 128; ++dstX) {
      // BMPs are horizontal but the screen is vertical 
      const size_t srcI = srcY * 160 + srcX;
      const size_t dstI = dstY * 128 + dstX;
      const ILI9163C_color_18_t& pixel = srcPtr[srcI];
      if(pixel != k_magenta) {
        memcpy_P(screenBuffer + dstI, srcPtr + srcI, sizeof(ILI9163C_color_18_t));
      }
      --srcY;
    }
    --srcX;
  }

  delete sprite.pixels;
}

void Anim::Reset() {
  idx = 0;
  lastDrawMillis = millis();
}
