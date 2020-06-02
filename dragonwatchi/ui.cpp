#include "ui.h"
#include "anims.h"
#include "utils.h"
#include "sd.h"
#include "dragon.h"

namespace {
  bool isOpen = false;
  
  enum class Menus : uint8_t {
    Exit = 0,
    Feed,
    Count,
  };
  PROGMEM const char* k_menusStrings[ToIntegral(Menus::Count)] = {
    "exit",
    "feed",
  };
  
  Menus currentMenu;

  Anim* backgroundAnim;
  Anim* selectedAnim;
  Anim* menuAnims[ToIntegral(Menus::Count)];

  void EnterMenu() {
    isOpen = true;
    const std::string menuPrefix = "menu/";
    backgroundAnim = new Anim(LoadAnim((menuPrefix + "background").c_str()));
    selectedAnim = new Anim(LoadAnim((menuPrefix + "selected").c_str()));

    int i = 0;
    for(const char* menuString : k_menusStrings) {
      menuAnims[i++] = new Anim(LoadAnim((menuPrefix + menuString).c_str()));
    }
  }
  void ExitMenu() {
    isOpen = false;
    
    delete backgroundAnim;
    backgroundAnim = nullptr;
    delete selectedAnim;
    selectedAnim = nullptr;
    for(size_t i = 0; i < ToIntegral(Menus::Count); ++i) {
      delete menuAnims[i];
      menuAnims[i] = nullptr;
    }
  }
}

void SetupUI() {
  
}

void LoopUI() {
  if(!isOpen) {
    if(digitalRead(RED_BTN)) {
      isOpen = true;
      EnterMenu();
    }
  }

  if(isOpen) {
    backgroundAnim->Draw();

    if(digitalRead(RED_BTN)) {
      currentMenu = Menus((ToIntegral(currentMenu) + 1) % (ToIntegral(Menus::Count)));
    }

    const size_t width = 35;
    const size_t y = 85;
    for(size_t i = 0; i < ToIntegral(Menus::Count); ++i) {
      const size_t x = 3 + i * width + i * 3;
      menuAnims[i]->Draw(x,y);
      if(ToIntegral(currentMenu) == i) {
        selectedAnim->Draw(x,y);
      }
    }

    if(digitalRead(GREEN_BTN)) {
      switch(currentMenu) {
        case Menus::Exit: {
          ExitMenu();
          break;
        }
        case Menus::Feed: {
          targetState = DragonState::Feeding;
          ExitMenu();
          break;
        }
      }
    }

  }
}
