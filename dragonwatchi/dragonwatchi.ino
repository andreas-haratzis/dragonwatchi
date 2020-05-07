#include <HyperDisplay_KWH018ST01_4WSPI.h>

#include "Textures.h"

#define SERIAL_PORT Serial    // Allows users to easily change target serial port (e.g. SAMD21's SerialUSB)

#define PWM_PIN 27             // Pin definitions
#define CS_PIN 33
#define DC_PIN 21
#define SPI_PORT SPI
#define SPI_SPEED 32000000        // Requests host uC to use the fastest possible SPI speed up to 32 MHz

ILI9163C_color_18_t RED = {0x3f, 0x00, 0x00};

KWH018ST01_4WSPI myTFT;           // The KWH018ST01_4WSPI class is used for this breakout, and we will call our object myTFT

wind_info_t smallWindow;            // Create a window
ILI9163C_color_18_t smallWindowMem[128*160];

#define RED_BTN 39
#define GREEN_BTN 36
#define BLUE_BTN 4
#define YELLOW_BTN 5

void setup() {
  SERIAL_PORT.begin(115200);

  SERIAL_PORT.println("Animation test!");

  pinMode(RED_BTN, INPUT);
  pinMode(GREEN_BTN, INPUT);
  pinMode(BLUE_BTN, INPUT);
  pinMode(YELLOW_BTN, INPUT);

  myTFT.begin(DC_PIN, CS_PIN, PWM_PIN, SPI_PORT, SPI_SPEED);
  myTFT.clearDisplay();

  myTFT.setWindowMemory(&smallWindow, (color_t)smallWindowMem, 160*128);    // Make sure that the window knows about the memory

  smallWindow.xMin = 0;              // Set the paramters to match the buffer size
  smallWindow.yMin = 0;      
  smallWindow.xMax = 127;         // See, 16+0 would be one pixel, so 16+31 is actually 32 pixels wide
  smallWindow.yMax = 159;         // and this means 64 pixels tall.. so now our small window can be filled with 32*64 pixels
  
  myTFT.pCurrentWindow = &smallWindow;    // Switch curent window to small window
  myTFT.buffer();                         // Set the current window to buffer mode
  myTFT.show();                           // Show all the changes at conc
}

int frameIdx = -1;
void loop() {

  frameIdx = (frameIdx + 1) % 10;

  const ILI9163C_color_18_t* srcPtr = NULL;
  
  switch(frameIdx) {
    case 0:
      srcPtr = frame1;
      break;
    case 1:
      srcPtr = frame2;
      break;
    case 2:
      srcPtr = frame3;
      break;
    case 3:
      srcPtr = frame4;
      break;
    case 4:
      srcPtr = frame5;
      break;
    case 5:
      srcPtr = frame6;
      break;
    case 6:
      srcPtr = frame7;
      break;
    case 7:
      srcPtr = frame8;
      break;
    case 8:
      srcPtr = frame9;
      break;
    case 9:
      srcPtr = frame10;
      break;
  }
  memcpy_P(smallWindowMem, srcPtr, 160*128*3);  // Necessary casts and dereferencing, just copy.
  myTFT.show();                           // Show all the changes at conc
  //delay(50);

  if(digitalRead(RED_BTN)) {
      SERIAL_PORT.println("Red");
  }
  if(digitalRead(GREEN_BTN)) {
      SERIAL_PORT.println("Green");
  }
  if(digitalRead(BLUE_BTN)) {
      SERIAL_PORT.println("Blue");
  }
  if(digitalRead(YELLOW_BTN)) {
      SERIAL_PORT.println("Yellow");
  }
}
