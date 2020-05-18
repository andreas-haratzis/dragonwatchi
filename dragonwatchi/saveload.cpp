
#include "saveload.h"
#include <Preferences.h>

WorldState world;

void save() {
  bool whichSaveBit = false;

  Preferences preferences;
  bool exists = preferences.begin("dragonwatchi", false);

  if (exists) {
    whichSaveBit = preferences.getBool("which_save");
  }

  // To avoid corruption, write bottom up: write the world in an unused block, and then flip the final bit to use it
  whichSaveBit = !whichSaveBit;

  const char* saveKey;
  if (whichSaveBit) {
    saveKey = "save_a";
  } else {
    saveKey = "save_b";
  }

  preferences.putBytes(saveKey, &world, sizeof(WorldState));

  preferences.end();

  Preferences preferences2;
  preferences2.begin("dragonwatchi", false);
  preferences2.putBool("which_save", whichSaveBit);
  preferences2.end();

  Serial.println("Saved!");
}

void load() {
  Preferences preferences;
  const bool exists = preferences.begin("dragonwatchi", true);
  
  if (!exists) {
    Serial.println("No save: dragonwatchi nvm namespace wasn't found. Starting from scratch");
    preferences.end();
    return;
  }
  
  const bool whichSaveBit = preferences.getBool("which_save");
  const char* saveKey;
  if (whichSaveBit) {
    saveKey = "save_a";
  } else {
    saveKey = "save_b";
  }
  
  const size_t saveLen = preferences.getBytesLength(saveKey);
  if (saveLen <= 0) {
    Serial.println("No save: empty save length. Starting from scratch");
    preferences.end();
    return;
  }
  
  void* buffer = malloc(saveLen);
  if (preferences.getBytes(saveKey, buffer, saveLen) != saveLen) {
    Serial.println("No save: size mismatchs");
    preferences.end();
    free(buffer);
    return;
  }
  
  const uint16_t& version = *((const uint16_t*)buffer);
  
  switch (version) {
    case k_latestVersion: {
        Serial.println("Loading latest state");
        memcpy(&world, buffer, saveLen);
        break;
      }
    default: {
        Serial.print("No save: read invalid version ");
        Serial.println(version);
        break;
      }
  }
  
  free(buffer);
  preferences.end();
}


/*
HungerState GetHungerState(int8_t hunger) {
#define IFHUNGER(hungerstate) if(hunger < ToIntegral(hungerstate)) { return hungerstate; }
  IFHUNGER(HungerState::Sated)
  IFHUNGER(HungerState::Hungry)
  IFHUNGER(HungerState::Famished)
  IFHUNGER(HungerState::Starving)
  IFHUNGER(HungerState::Dead)
#undef IFHUNGER
}
*/
