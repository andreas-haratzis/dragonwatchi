#ifndef UTILS_H
#define UTILS_H

#include <HyperDisplay_KWH018ST01_4WSPI.h>

#define RED_BTN 15
#define GREEN_BTN 32
#define BLUE_BTN 14
#define YELLOW_BTN 22
#define ZERO_BTN 0

bool operator==(const ILI9163C_color_18_t& a, const ILI9163C_color_18_t& b);
bool operator!=(const ILI9163C_color_18_t& a, const ILI9163C_color_18_t& b);

const extern ILI9163C_color_18_t k_magenta;

extern unsigned long lastFrameDurationMillis;

extern KWH018ST01_4WSPI myTFT;

template<typename E>
constexpr auto ToIntegral(E e) -> typename std::underlying_type<E>::type 
{
   return static_cast<typename std::underlying_type<E>::type>(e);
}

std::string ToLower(const std::string str);

void Halt();

#endif
