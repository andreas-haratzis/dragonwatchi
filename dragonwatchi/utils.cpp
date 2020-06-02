#include "utils.h"

bool operator==(const ILI9163C_color_18_t& a, const ILI9163C_color_18_t& b) {
  return a.r == b.r && a.g == b.g && a.b == b.b;
}
bool operator!=(const ILI9163C_color_18_t& a, const ILI9163C_color_18_t& b) {
  return !(a == b);
}

const ILI9163C_color_18_t k_magenta{255, 0, 255};

unsigned long lastFrameDurationMillis;

void Halt() {
  // TODO: Keep screensaver working
  while(true) {
    delay(1000 * 60 * 60);
  }
}

std::string ToLower(const std::string& str) {
  std::string lowerStr(str);
  std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
    [](unsigned char c){ return std::tolower(c); });
  return lowerStr;
}

KWH018ST01_4WSPI myTFT;
