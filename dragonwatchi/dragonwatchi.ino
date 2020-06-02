#include <HyperDisplay_KWH018ST01_4WSPI.h>

#include "sprites.h"
#include "anims.h"
#include "utils.h"
#include "saveload.h"
#include "ui.h"
#include "dragon.h"
#include "sd.h"

#define PWM_PIN 27             // Pin definitions
#define CS_PIN 33
#define DC_PIN 21
#define SPI_PORT SPI
#define SPI_SPEED 32000000        // Requests host uC to use the fastest possible SPI speed up to 32 MHz

unsigned long hungerTickCountdownMillis = 1000 * 5;//* 60 * 60;

Anim* anim_background = NULL;

void setup() {
  Serial.begin(115200);
  Serial.println("Dragon watchi v0");

  Serial.print("Free heap: ");
  Serial.println(heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));
      
  pinMode(RED_BTN, INPUT);
  pinMode(GREEN_BTN, INPUT);
  pinMode(BLUE_BTN, INPUT);
  pinMode(YELLOW_BTN, INPUT);
  pinMode(ZERO_BTN, INPUT);

  myTFT.begin(DC_PIN, CS_PIN, PWM_PIN, SPI_PORT, SPI_SPEED);
  myTFT.clearDisplay();

  myTFT.setCurrentWindowMemory((color_t)screenBuffer, 160 * 128);  // Make sure that the window knows about the memory
  myTFT.show();                           // Show all the changes at conc

  InitSD();
  anim_background = new Anim(LoadAnim("background"));
  InitDragon();
  
  load();
  //save();

  tempDragonState->SwitchTo(DragonState::Sleeping, world.dragonState);

  SetupUI();
}

void loop() {
  const unsigned long frameStartMillis = millis();
  anim_background->Draw();
  
  targetState = world.dragonState;
  
  tempDragonState->Loop(world.dragonState);

  if (!digitalRead(ZERO_BTN)) {
    world.hunger--;
    Serial.print("Reduced hunger to ");
    Serial.println(world.hunger);
  }

  if (lastFrameDurationMillis > hungerTickCountdownMillis) {
    hungerTickCountdownMillis = 1000 * 60 * 60;
    world.hunger--;
    Serial.print("Reduced hunger to ");
    Serial.println(world.hunger);
    save();
  } else {
    hungerTickCountdownMillis -= lastFrameDurationMillis;
  }

  if (world.hunger <= ToIntegral(HungerState::Dead)) {
    Serial.println("Draggo starved to death :(");
    targetState = DragonState::Dead;
  }

  LoopUI();

  if (targetState != world.dragonState) {
    Serial.print("Switching from ");
    Serial.print(ToIntegral(world.dragonState));
    Serial.print(" to ");
    Serial.println(ToIntegral(targetState));

    tempDragonState->SwitchTo(world.dragonState, targetState);
    world.dragonState = targetState;

    if(world.dragonState == DragonState::Vibing) {
      save();
    }
  }

  myTFT.show();                           // Show all the changes at conc

  unsigned long frameEndMillis = millis();
  unsigned long frameElapsedMillis = frameEndMillis - frameStartMillis;

  const unsigned long k_minFrameMillis = 33;
  if (frameElapsedMillis < k_minFrameMillis) {
    delay(k_minFrameMillis - frameElapsedMillis);
    frameEndMillis = millis();
    frameElapsedMillis = frameEndMillis - frameStartMillis;
  }

  lastFrameDurationMillis = frameElapsedMillis;
}
