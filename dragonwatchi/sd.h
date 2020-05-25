#ifndef SD_H
#define SD_H

#include "sprites.h"
#include "anims.h"
#include <FS.h>

void InitSD();

Sprite LoadSprite(File& file);
Anim LoadAnim(const char* name);

#endif
