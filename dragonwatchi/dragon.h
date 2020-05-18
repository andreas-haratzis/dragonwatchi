#ifndef DRAGON_H 
#define DRAGON_H

#include "anims.h"
#include "saveload.h"

struct TempDragonStateSleeping {
  SleepingAnim anim;
  unsigned int screensaverCountdownMillis = 1000 * 8;

  void Loop();
};

struct TempDragonStateWoke {
  WokeAnim anim;
  uint16_t backToSleepCountdown = 3000;

  void Loop();
};

struct TempDragonStateVibing {
  uint16_t backToSleepCountdown = 1000 * 60 * 5;
  VibingHappyAnim happyAnim;
  VibingNeutralAnim neutralAnim;
  VibingSadAnim sadAnim;
  VibingDyingAnim dyingAnim;
  VibingHungerAnim vibingHungerAnim;
  DyingAnim dyingWarningAnim;

  void Loop();
};

struct TempDragonStateDead {
  DeadAnim anim;

  void Loop();
};

union TempDragonStates {
  TempDragonStateSleeping sleeping;
  TempDragonStateWoke woke;
  TempDragonStateVibing vibing;
  TempDragonStateDead dead;
  
  TempDragonStates();
  ~TempDragonStates();
  
  void SwitchTo(DragonState from, DragonState to);
  void Loop(DragonState which);
};

extern TempDragonStates tempDragonState;
extern DragonState targetState;

#endif
