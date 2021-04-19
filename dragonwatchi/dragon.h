#ifndef DRAGON_H 
#define DRAGON_H

#include "anims.h"
#include "sd.h"
#include "flappydrag.h"

#include <functional>

enum class DragonState : uint8_t {
  Sleeping = 0,
  Woke,
  Vibing,
  Feeding,
  Playing,
  Dead, // :(
};

enum class HungerState : int8_t {
  //Orb = 100,
  //Fat = 75,
  //Thicc = 50,
  //Stuffed = 25,
  //Full = 10,
  Sated = 0,
  Hungry = -8,
  Famished = -25,
  Starving = -50,
  Dead = -100
};

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

struct TempDragonStateFeeding {
  Anim anim = LoadAnim("feeding");

  void Loop();
};

struct TempDragonStatePlaying {
  Flappydrag game;
  
  void Loop();
};

union TempDragonStates {
  TempDragonStateSleeping sleeping;
  TempDragonStateWoke woke;
  TempDragonStateVibing vibing;
  TempDragonStateDead dead;
  TempDragonStateFeeding feeding;
  TempDragonStatePlaying playing;
  
  TempDragonStates();
  ~TempDragonStates();
  
  void SwitchTo(DragonState from, DragonState to);
  void Loop(DragonState which);
};

void InitDragon();

extern TempDragonStates* tempDragonState;
extern DragonState targetState;

#endif
