#include "sd.h"

#include "utils.h"
#include "anims.h"
#include <SD.h>
#include <FS.h>
#include <rom/miniz.h>
#include <esp_heap_caps.h>

#define SD_SELECT_PIN 16

namespace {
  const char* k_spritePath = "/dragonwatchi/sprites/";
  const char* k_spriteExtension = ".bmp";
    
  void EnterSD() {
    digitalWrite(SD_SELECT_PIN, false);
  }
  void ExitSD() {
    digitalWrite(SD_SELECT_PIN, true);
  }

  void SDPanic() {
    Serial.println("SD Panic!");
    ExitSD();

    size_t targetX = 0;
    size_t targetY = 0;
    for(size_t sourceY = 0; sourceY < sprite_no_sd.height; ++sourceY) {
      targetX = 0;
      for(size_t sourceX = 0; sourceX < sprite_no_sd.width; ++sourceX) {
        const size_t sourceI = sourceY * sprite_no_sd.height + sourceX;
        const size_t targetI = targetY * 128 + targetX;
        const ILI9163C_color_18_t& pixel = sprite_no_sd.pixels[sourceI];
        if(pixel != k_magenta) {
          memcpy_P(screenBuffer + targetI, &pixel, sizeof(ILI9163C_color_18_t));
        }
        ++targetX;
      }
      ++targetY;
    }

    myTFT.show();
    Halt();
  }

  #pragma pack(push, 1)

  // See https://stackoverflow.com/questions/14279242/read-bitmap-file-into-structure

  struct BitmapFileHeader
  {
    uint16_t bfType;  //specifies the file type
    uint32_t bfSize;  //specifies the size in bytes of the bitmap file
    uint16_t bfReserved1;  //reserved; must be 0
    uint16_t bfReserved2;  //reserved; must be 0
    uint32_t bfOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
  };

  struct BitmapInfoHeader
  {
    uint32_t biSize;  //specifies the number of bytes required by the struct
    int32_t biWidth;  //specifies width in pixels
    int32_t biHeight;  //species height in pixels
    uint16_t biPlanes; //specifies the number of color planes, must be 1
    uint16_t biBitCount; //specifies the number of bit per pixel
    uint32_t biCompression;//spcifies the type of compression
    uint32_t biSizeImage;  //size of image in bytes
    int32_t biXPelsPerMeter;  //number of pixels per meter in x axis
    int32_t biYPelsPerMeter;  //number of pixels per meter in y axis
    uint32_t biClrUsed;  //number of colors used by th ebitmap
    uint32_t biClrImportant;  //number of colors that are important
  };
  
  #pragma pack(pop)
}


void InitSD() {
  pinMode(SD_SELECT_PIN, OUTPUT);
  
  EnterSD();
  
  if(!SD.begin(SS, SPI, 4000000, "/sd", 16)){
    Serial.println("Card Mount Failed");
    SDPanic();
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    SDPanic();
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024 * 1024);
  Serial.printf("SD Card Size: %lluGB\n", cardSize);

  ExitSD();
}

Sprite LoadSprite(File& file) {
  // This func takes about 10ms

  EnterSD();
  
  unsigned char* buffer = (unsigned char*)malloc(file.size());
  if(!buffer) {
    Serial.printf("Failed to malloc %llu for %s\n", file.size(), file.name());
    SDPanic();
  }
  
  file.seek(0);
  
  if(!file.read((uint8_t*)buffer, file.size())) {
    Serial.printf("Failed to read %s\n", file.name());
    SDPanic();
  }

  // Must hold the largest possible BMP
  mz_ulong bmpLen = 160*128*3 + 128;
  unsigned char *bmpMem = (unsigned char *)malloc(160*128*3 + 1024);
  bmpLen = tinfl_decompress_mem_to_mem(bmpMem, bmpLen, buffer, file.size(), TINFL_FLAG_PARSE_ZLIB_HEADER);

  if(bmpLen == TINFL_DECOMPRESS_MEM_TO_MEM_FAILED) {
    Serial.printf("Failed to inflate %s\n", file.name());
    SDPanic();
  }

  free(buffer);

  size_t size = 0;
  size_t width = 0;
  size_t height = 0;
  size_t pixelsOffset = 0;
  {
    const BitmapFileHeader& fileHeader = *(const BitmapFileHeader*)(bmpMem);
  
    if (fileHeader.bfType != 0x4D42)
    {
      Serial.print("Invalid bmp magic number for ");
      Serial.println(file.name());
      SDPanic();
    }
  
    const BitmapInfoHeader& infoHeader= *(const BitmapInfoHeader*)(bmpMem + sizeof(BitmapFileHeader));

    size = infoHeader.biSizeImage;
    width = infoHeader.biWidth;
    height = infoHeader.biHeight;
    pixelsOffset = fileHeader.bfOffBits;
  }

  // Avoid allocating too much mem: Use the existing bmp allocation 
  memmove(bmpMem, bmpMem + pixelsOffset, size);
  bmpMem = (unsigned char*)realloc(bmpMem, size);

  // BMPs are in BGR, we want RGB
  for(size_t i = 0; i < size / 3; ++i) {
    const uint8_t t = bmpMem[i*3];
    bmpMem[i*3] = bmpMem[i*3 + 2];
    bmpMem[i*3 + 2] = t;
  }

  ExitSD();
    
  // FIXME: Leak here! where do we cleanup the pixels? destructor?
  Sprite newSprite{width, height, (const ILI9163C_color_18*)bmpMem};
  
  return newSprite;
}

Anim LoadAnim(const char* name) {
  EnterSD();

  char* path = new char[strlen(k_spritePath) + strlen(name) + 1];
  sprintf(path, "%s%s", k_spritePath, name);

  File root = SD.open(path);
  if(!root){
    Serial.print("Failed to open ");
    Serial.println(path);
    SDPanic();
  }

  if(!root.isDirectory()){
    Serial.print(path);
    Serial.println(" is not a directory");
    SDPanic();
  }

  Files sprites;
  
  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("Ignoring directory ");
      Serial.println(file.name());
    } else {
      std::string name(file.name());
      if(name.size() > 4 && name[name.size() - 3] == '.' && name[name.size() - 2] == 'g' && name[name.size() - 1] == 'z') {
        sprites.push_back(file);
      } else {
        Serial.print("Ignoring non GZ ");
        Serial.println(file.name());
      }
    }
    file = root.openNextFile();
  }
  
  delete path;
  ExitSD();

  Anim newAnim{std::move(sprites), 0};
  return newAnim;
}
