#include "dragon.h"
#include "utils.h"

TempDragonStates tempDragonState;
DragonState targetState = DragonState::Sleeping;

TempDragonStates::TempDragonStates()
  : sleeping() {
}
TempDragonStates::~TempDragonStates() {
}

void TempDragonStates::SwitchTo(DragonState from, DragonState to) {
  switch (from) {
    case DragonState::Sleeping: {
      sleeping.~TempDragonStateSleeping();
      break;
    }
    case DragonState::Woke: {
      woke.~TempDragonStateWoke();
      break;
    }
    case DragonState::Vibing: {
      vibing.~TempDragonStateVibing();
      break;
    }
    case DragonState::Dead: {
      dead.~TempDragonStateDead();
      break;
    }
  }

  switch (to) {
    case DragonState::Sleeping: {
      new (&sleeping) TempDragonStateSleeping();
      break;
    }
    case DragonState::Woke: {
      new (&woke) TempDragonStateWoke();
      break;
    }
    case DragonState::Vibing: {
      new (&vibing) TempDragonStateVibing();
      break;
    }
    case DragonState::Dead: {
      new (&dead) TempDragonStateDead();
      break;
    }
  }
}

void TempDragonStates::Loop(DragonState which) {
  switch (which) {
    case DragonState::Sleeping: {
      sleeping.Loop();
      break;
    }
    case DragonState::Woke: {
      woke.Loop();
      break;
    }
    case DragonState::Vibing: {
      vibing.Loop();
      break;
    }
    case DragonState::Dead: {
      dead.Loop();
      break;
    }
  }
}


void TempDragonStateSleeping::Loop() {
  if(lastFrameDurationMillis > screensaverCountdownMillis) {
    if (digitalRead(YELLOW_BTN)) {
      screensaverCountdownMillis = 1000 * 8;
      delay(500);
      myTFT.setBacklight(255);
    } else {
      memset(screenBuffer, 255, 160 * 128 * sizeof(ILI9163C_color_18_t));
      myTFT.setBacklight(0);
    }
  } else {
    screensaverCountdownMillis -= lastFrameDurationMillis;

    anim.Draw();
    if (digitalRead(YELLOW_BTN)) {
      targetState = DragonState::Woke;
      delay(500);
    }
  }
}

void TempDragonStateWoke::Loop() {
  anim.Draw();

  if (digitalRead(YELLOW_BTN)) {
    targetState = DragonState::Vibing;
  }
  
  if(lastFrameDurationMillis > backToSleepCountdown) {
    targetState = DragonState::Sleeping;
  } else {
    backToSleepCountdown -= lastFrameDurationMillis;
  }
}

void TempDragonStateVibing::Loop() {
  if(world.hunger <= ToIntegral(HungerState::Starving)) {
    dyingAnim.Draw();
  } else if(world.hunger <= ToIntegral(HungerState::Famished)) {
    sadAnim.Draw();
  } else {
    neutralAnim.Draw();
  }

  if(world.hunger <= ToIntegral(HungerState::Hungry)) {
    vibingHungerAnim.Draw();
  }

  if(world.hunger <= ToIntegral(HungerState::Starving)) {
    dyingWarningAnim.Draw();
  }

  if (digitalRead(YELLOW_BTN)) {
    backToSleepCountdown = 1000 * 60 * 5;
  }else if(lastFrameDurationMillis > backToSleepCountdown) {
    targetState = DragonState::Sleeping;
  } else {
    backToSleepCountdown -= lastFrameDurationMillis;
  }
}

void TempDragonStateDead::Loop() {
  anim.Draw();
}
