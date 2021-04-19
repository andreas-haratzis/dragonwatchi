#include "flappydrag.h"
#include "utils.h"
#include "dragon.h"

namespace {
  PROGMEM const char* k_stateAnims[ToIntegral(Flappydrag::State::Count)] = {
    "flappydrag/intro",
    "flappydrag/flying",
    "flappydrag/crash",
    "flappydrag/outro",
  };
}

Flappydrag::Flappydrag() {
  
}

void Flappydrag::Loop() {
  // TODO: should we his hitbox bigger when he's fat?
  
  State targetState = state;

  switch(state) {
    case State::Intro: {
      backgroundAnim.Draw();
      stateAnim.Draw();
      if(stateAnim.loops > 0) {
        targetState = State::Flying;
      }
      break;
    }
    case State::Outro: {
      backgroundAnim.Draw();
      stateAnim.Draw();
      if(stateAnim.loops > 0) {
        ::targetState = DragonState::Vibing;
      }
      break;
    }
    case State::Flying: {
      if (digitalRead(YELLOW_BTN)) {
        velocity = -15;
      }

      const float deltaSeconds = (((float)lastFrameDurationMillis) / 1000.f) * 3;
      velocity += gravity * deltaSeconds;
      y += velocity * deltaSeconds;

      const int planeHeight = 35;
      
      if(y < (planeHeight/2)) {
        y = (planeHeight/2);
        velocity = 0;
      }
      
      if(y > 128) {
        targetState = State::Crash;
      }
      
      const int x = 10;
      const int wallGap = 40;
      const int wallWidth = 30;
      
      if(x >= wallX && x < wallX + wallWidth) {
        if(y - planeHeight/2 < wallY - wallGap/2 || y + planeHeight/2 > wallY + wallGap/2) {
          targetState = State::Crash;
        }
      }

      wallX -= deltaSeconds * 20;

      if(wallX < -40) {
        wallX = 128 + 40;
        wallY = random(40,100);
      }
      
      backgroundAnim.Draw();
      wallUpperAnim.Draw(wallX, wallY - 128 - wallGap/2);
      wallLowerAnim.Draw(wallX, wallY + wallGap/2);
      stateAnim.Draw(x,y - (35/2));
      
      break;
    }
    case State::Crash: {
      backgroundAnim.Draw();
      stateAnim.Draw(10,y - (70/2));
      if(stateAnim.loops > 0) {
        targetState = State::Outro;
      }
      break;
    }
  }
  
  if(targetState != state) {
    state = targetState;
    millisInCurrentState = 0;
    Serial.print("Moving to target state ");
    Serial.print(ToIntegral(targetState));
    Serial.print(" ");
    Serial.println(k_stateAnims[ToIntegral(targetState)]);
    stateAnim = LoadAnim(k_stateAnims[ToIntegral(targetState)]);
  } else {
    millisInCurrentState += lastFrameDurationMillis;
  }
}
