#ifndef DRAGON_H 
#define DRAGON_H

#include "anims.h"
#include "saveload.h"
#include "sd.h"

#include <functional>

struct TempDragonStateSleeping {
  Anim anim = LoadAnim("sleeping");
  unsigned int screensaverCountdownMillis = 1000 * 8;

  void Loop();
};

struct TempDragonStateWoke {
  Anim anim = LoadAnim("woke");
  uint16_t backToSleepCountdown = 3000;

  void Loop();
};

struct TempDragonStateVibing {
  uint16_t backToSleepCountdown = 1000 * 60 * 5;
  Anim happyAnim = LoadAnim("vibing/happy");
  Anim neutralAnim = LoadAnim("vibing/neutral");
  Anim sadAnim = LoadAnim("vibing/sad");
  Anim dyingAnim = LoadAnim("vibing/dying");
  Anim vibingHungerAnim = LoadAnim("vibing/hunger");
  Anim dyingWarningAnim = LoadAnim("vibing/dying");

  void Loop();
};

struct TempDragonStateDead {
  Anim anim = LoadAnim("dead");

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

void InitDragon();

extern TempDragonStates* tempDragonState;
extern DragonState targetState;

#endif
