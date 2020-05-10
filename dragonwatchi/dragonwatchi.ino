#include <HyperDisplay_KWH018ST01_4WSPI.h>

#include "sprites.h"
#include "anims.h"

#define SERIAL_PORT Serial    // Allows users to easily change target serial port (e.g. SAMD21's SerialUSB)

#define PWM_PIN 27             // Pin definitions
#define CS_PIN 33
#define DC_PIN 21
#define SPI_PORT SPI
#define SPI_SPEED 32000000        // Requests host uC to use the fastest possible SPI speed up to 32 MHz

ILI9163C_color_18_t RED = {0x3f, 0x00, 0x00};

KWH018ST01_4WSPI myTFT;           // The KWH018ST01_4WSPI class is used for this breakout, and we will call our object myTFT

wind_info_t screenBufferWindow;            // Create a window

#define RED_BTN 39
#define GREEN_BTN 36
#define BLUE_BTN 5
#define YELLOW_BTN 4

void setup() {
  SERIAL_PORT.begin(115200);

  SERIAL_PORT.println("Dragon watchi v0");

  //pinMode(RED_BTN, INPUT);
  //pinMode(GREEN_BTN, INPUT);
  //pinMode(BLUE_BTN, INPUT);
  pinMode(YELLOW_BTN, INPUT);

  myTFT.begin(DC_PIN, CS_PIN, PWM_PIN, SPI_PORT, SPI_SPEED);
  myTFT.clearDisplay();

  myTFT.setWindowMemory(&screenBufferWindow, (color_t)screenBuffer, 160*128);    // Make sure that the window knows about the memory

  screenBufferWindow.xMin = 0;              // Set the paramters to match the buffer size
  screenBufferWindow.yMin = 0;      
  screenBufferWindow.xMax = 127;         // See, 16+0 would be one pixel, so 16+31 is actually 32 pixels wide
  screenBufferWindow.yMax = 159;         // and this means 64 pixels tall.. so now our small window can be filled with 32*64 pixels
  
  myTFT.pCurrentWindow = &screenBufferWindow;    // Switch curent window to small window
  myTFT.buffer();                         // Set the current window to buffer mode
  myTFT.show();                           // Show all the changes at conc
}

SleepingAnim sleepingAnim;
WokeAnim wokeAnim;

class WorldState {
  DragonState dragonState;
};

class SavedState {
  uint16_t vers = 1;
  
  // Buffer our saves, to avoid corruption 
  bool isFirstWorldValid;
  WorldState firstWorld;
  WorldState secondWorld;
};

enum class DragonState : uint8_t {
  Sleeping = 0,
  Woke,
};

DragonState dragonState = DragonState::Sleeping;

void loop() {

  const ILI9163C_color_18_t* srcPtr = NULL;

  switch(dragonState) {
    case DragonState::Sleeping:
      sleepingAnim.Draw();
      
      if(digitalRead(YELLOW_BTN)) {
        dragonState = DragonState::Woke;
      }
    break;
    case DragonState::Woke:
      wokeAnim.Draw();
      myTFT.show();
      delay(3000);
      dragonState = DragonState::Sleeping;
      sleepingAnim.idx = 0;
      sleepingAnim.lastDrawMillis = millis();
    break;
  }
  
  myTFT.show();                           // Show all the changes at conc
  //delay(100);

  //if(digitalRead(RED_BTN)) {
  //    SERIAL_PORT.println("Red");
  //}
  //if(digitalRead(GREEN_BTN)) {
  //    SERIAL_PORT.println("Green");
  //}
  //if(digitalRead(BLUE_BTN)) {
  //    SERIAL_PORT.println("Blue");
  //}
  
}
