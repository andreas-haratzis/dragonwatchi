#ifndef FLAPPYDRAG_H  
#define FLAPPYDRAG_H  

#include "sd.h"
#include <cstdint>

class Flappydrag {
  public:
  enum class State : uint8_t {
    Intro = 0,
    Flying,
    Crash,
    Outro,
    Count
  };

  State state = State::Intro;
  size_t millisInCurrentState = 0;
  Anim stateAnim = LoadAnim("flappydrag/intro");
  Anim backgroundAnim = LoadAnim("flappydrag/background");
  Anim wallUpperAnim = LoadAnim("flappydrag/walls/upper");
  Anim wallLowerAnim = LoadAnim("flappydrag/walls/lower");

  float y = 128/2.f;
  float velocity = 0.f;
  const float gravity = 10.f;

  float wallX = 128 + 40;
  float wallY = 128/2;
  
  Flappydrag();

  void Loop();
};


#endif
