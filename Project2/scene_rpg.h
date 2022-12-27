#define _CRT_SECURE_NO_WARNINGS

// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

#ifndef SCENE_RPG_H
#define SCENE_RPG_H
#include "game.h"

Scene scene_rpg_create(void);

#endif
