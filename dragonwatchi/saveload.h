#ifndef SAVELOAD_H 
#define SAVELOAD_H

#include "dragon.h"
#include <cstdint>

void save();
void load();

// Increment when versions are incompatible. Next load will WIPE the save. Should probably add migration code
const uint16_t k_latestVersion = 4;

// World state is persistent: saved to eeprom occasionally, loaded on startup
struct WorldState {
  // Always keep version as first member, loading code manually checks the first bytes for version
  uint16_t k_version = k_latestVersion;

  DragonState dragonState = DragonState::Sleeping;
  int8_t hunger = 0;
};

extern WorldState world;

#endif
