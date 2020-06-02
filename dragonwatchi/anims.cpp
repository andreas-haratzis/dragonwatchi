#include "anims.h"
#include "utils.h"
#include "sd.h"

ILI9163C_color_18_t screenBuffer[128*160] = {0};

Anim::Anim(Files&& sprites, size_t loopIdx)
  : lastDrawMillis{millis()}
  , sprites{std::move(sprites)}
  , loopIdx(loopIdx) {
}

void Anim::Draw(size_t x, size_t y) {
  const unsigned long currMillis = millis();
  if(currMillis - lastDrawMillis > 1000) {
    idx = (idx + 1) % sprites.size();
    if(idx == 0) {
      if(loopIdx == k_invalidLoopIdx) {
        idx = sprites.size() - 1;
      } else {
        idx = loopIdx;
      }
      ++loops;
    }
    lastDrawMillis = currMillis;
  }

  Sprite sprite = LoadSprite(sprites[idx]);

  const ILI9163C_color_18_t* srcPtr = sprite.pixels;
  
  // BMPs are landscape but the screen is portrait
  // and flipped

  size_t srcX = sprite.width - 1;
  for(size_t dstY = x; dstY < 160; ++dstY) {
    size_t srcY = sprite.height - 1;
    for(size_t dstX = y; dstX < 128; ++dstX) {
      const size_t srcI = srcY * sprite.width + srcX;
      const size_t dstI = dstY * 128 + dstX;
      const ILI9163C_color_18_t& pixel = srcPtr[srcI];
      if(pixel != k_magenta) {
        memcpy_P(screenBuffer + dstI, srcPtr + srcI, sizeof(ILI9163C_color_18_t));
      }
      
      if(srcY <= 0) {
        break;
      }
      --srcY;
    }
    
    if(srcX <= 0) {
      break;
    }
    --srcX;
  }

  delete sprite.pixels;
}

void Anim::Reset() {
  idx = 0;
  lastDrawMillis = millis();
}
