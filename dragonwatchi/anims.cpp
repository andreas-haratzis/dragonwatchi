#include "anims.h"
#include "utils.h"
#include "sd.h"

namespace {
  unsigned long GetMillisBetweenSprites(const File& before, const File& after) {
    const char* beforeStr = strrchr(before.name(), '/');
    const char* afterStr = strrchr(after.name(), '/');
    if(beforeStr == nullptr) {
      beforeStr = before.name();
    } else {
      beforeStr += 1;
    }
    if(afterStr == nullptr) {
      afterStr = after.name();
    } else {
      afterStr += 1;
    }
    unsigned long beforeMillis = std::strtoul(beforeStr, nullptr, 10);
    unsigned long afterMillis = std::strtoul(afterStr, nullptr, 10);
    return afterMillis - beforeMillis;
  }
}

ILI9163C_color_18_t screenBuffer[128*160] = {0};

Anim::Anim(Files&& sprites, size_t loopIdx)
  : lastDrawMillis{millis()}
  , sprites{std::move(sprites)}
  , loopIdx(loopIdx) {

  if(sprites.size() > 1) {
    millisRemaining = GetMillisBetweenSprites(sprites[0], sprites[1]);
  }
}

void Anim::Draw(int x, int y) {
  const unsigned long currMillis = millis();
  if(currMillis - lastDrawMillis > millisRemaining) {
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
    millisRemaining = 0;

    if(idx + 1 < sprites.size()) {
      millisRemaining = GetMillisBetweenSprites(sprites[idx], sprites[idx + 1]);
    }
  }

  Sprite sprite = LoadSprite(sprites[idx]);

  const ILI9163C_color_18_t* srcPtr = sprite.pixels;
  
  for(int srcX = 0; srcX < sprite.width; ++srcX) {
    for(int srcY = 0; srcY < sprite.height; ++srcY) {
      int dstX = x + srcX;
      int dstY = y + srcY;
      LandscapeCoordToPortrait(dstX, dstY);
      
      if(dstX >= 128 || dstY >= 160 || dstX < 0 || dstY < 0) {
        continue;
      }

      // BMP buffer reads left-to-right, but bottom-to-top
      const int srcI = (sprite.height - 1 - srcY) * sprite.width + srcX;
      const ILI9163C_color_18_t& pixel = srcPtr[srcI];
      
      if(pixel != k_magenta) {
        const int dstI = dstY * 128 + dstX;
        memcpy_P(screenBuffer + dstI, &pixel, sizeof(ILI9163C_color_18_t));
      }
    }
  }

  delete sprite.pixels;
}

void Anim::Reset() {
  idx = 0;
  lastDrawMillis = millis();
}
