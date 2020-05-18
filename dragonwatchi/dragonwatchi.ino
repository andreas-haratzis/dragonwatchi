#include <HyperDisplay_KWH018ST01_4WSPI.h>

#include "sprites.h"
#include "anims.h"
#include "utils.h"
#include "saveload.h"
#include "ui.h"
#include "dragon.h"

#define SERIAL_PORT Serial    // Allows users to easily change target serial port (e.g. SAMD21's SerialUSB)

#define PWM_PIN 27             // Pin definitions
#define CS_PIN 33
#define DC_PIN 21
#define SPI_PORT SPI
#define SPI_SPEED 32000000        // Requests host uC to use the fastest possible SPI speed up to 32 MHz

wind_info_t screenBufferWindow;            // Create a window

unsigned long hungerTickCountdownMillis = 1000 * 5;//* 60 * 60;

void setup() {
  SERIAL_PORT.begin(115200);
  SERIAL_PORT.println("Dragon watchi v0");

  pinMode(YELLOW_BTN, INPUT);
  pinMode(ZERO_BTN, INPUT);
  pinMode(BLUE_BTN, INPUT);
  
  myTFT.begin(DC_PIN, CS_PIN, PWM_PIN, SPI_PORT, SPI_SPEED);
  myTFT.clearDisplay();

  myTFT.setWindowMemory(&screenBufferWindow, (color_t)screenBuffer, 160 * 128);  // Make sure that the window knows about the memory

  screenBufferWindow.xMin = 0;              // Set the paramters to match the buffer size
  screenBufferWindow.yMin = 0;
  screenBufferWindow.xMax = 127;         // See, 16+0 would be one pixel, so 16+31 is actually 32 pixels wide
  screenBufferWindow.yMax = 159;         // and this means 64 pixels tall.. so now our small window can be filled with 32*64 pixels

  myTFT.pCurrentWindow = &screenBufferWindow;    // Switch curent window to small window
  myTFT.buffer();                         // Set the current window to buffer mode
  myTFT.show();                           // Show all the changes at conc

  load();
  save();

  tempDragonState.SwitchTo(DragonState::Sleeping, world.dragonState);

  SetupUI();
}

void loop() {
  memcpy_P(screenBuffer, sprite_background.pixels, 160 * 128 * sizeof(ILI9163C_color_18_t));

  targetState = world.dragonState;

  const unsigned long frameStartMillis = millis();
  tempDragonState.Loop(world.dragonState);

  if (!digitalRead(ZERO_BTN)) {
    world.hunger--;
    SERIAL_PORT.print("Reduced hunger to ");
    SERIAL_PORT.println(world.hunger);
  }

  if (lastFrameDurationMillis > hungerTickCountdownMillis) {
    hungerTickCountdownMillis = 1000 * 60 * 60;
    world.hunger--;
    SERIAL_PORT.print("Reduced hunger to ");
    SERIAL_PORT.println(world.hunger);
    save();
  } else {
    hungerTickCountdownMillis -= lastFrameDurationMillis;
  }
  
  if(world.hunger <= ToIntegral(HungerState::Dead)) {
    SERIAL_PORT.println("Draggo starved to death :(");
    targetState = DragonState::Dead;
  }

  LoopUI();

  if (targetState != world.dragonState) {
    SERIAL_PORT.print("Switching from ");
    SERIAL_PORT.print(ToIntegral(world.dragonState));
    SERIAL_PORT.print(" to ");
    SERIAL_PORT.println(ToIntegral(targetState));
  
    tempDragonState.SwitchTo(world.dragonState, targetState);
    world.dragonState = targetState;
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
