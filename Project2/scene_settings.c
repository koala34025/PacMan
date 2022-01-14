﻿#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <math.h>
#include "scene_menu_object.h"
#include "scene_settings.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "scene_end.h"
#include "utility.h"
#include "shared.h"

// [HACKATHON 3-9]
// TODO: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.


// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

static Button btn2;
static Button mute;
static Button unmute;

static void draw(void){
	al_clear_to_color(al_map_rgb(0, 0, 0));
	
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2 - 175,
		SCREEN_H / 2 - 50,
		ALLEGRO_ALIGN_CENTER,
		"mute"
	);

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2 + 170,
		SCREEN_H / 2 - 50,
		ALLEGRO_ALIGN_CENTER,
		"unmute"
	);

	if (music_volume == 0) {
		al_draw_text(
			menuFont,
			al_map_rgb(255, 255, 255),
			SCREEN_W / 2 - 223,
			SCREEN_H / 2 - 49,
			ALLEGRO_ALIGN_CENTER,
			">"
		);
	}
	else {
		al_draw_text(
			menuFont,
			al_map_rgb(255, 255, 255),
			SCREEN_W / 2 + 105,
			SCREEN_H / 2 - 49,
			ALLEGRO_ALIGN_CENTER,
			">"
		);
	}

	drawButton(btn2);
	drawButton(mute);
	drawButton(unmute);
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	btn2.hovered = buttonHover(btn2, mouse_x, mouse_y);
	mute.hovered = buttonHover(mute, mouse_x, mouse_y);
	unmute.hovered = buttonHover(unmute, mouse_x, mouse_y);
}

static void on_mouse_down() {
	if (btn2.hovered)
		game_change_scene(scene_menu_create());
	if (mute.hovered) {
		//mute
		music_volume = 0;
		effect_volume = 0;
		game_log("mute");
	}
	if (unmute.hovered) {
		//unmute
		music_volume = 0.5;
		effect_volume = 0.5;
		game_log("unmute");
	}
}

static void destroy() {
	al_destroy_bitmap(btn2.default_img);
	al_destroy_bitmap(btn2.hovered_img);
	al_destroy_bitmap(mute.default_img);
	al_destroy_bitmap(mute.hovered_img);
	al_destroy_bitmap(unmute.default_img);
	al_destroy_bitmap(unmute.hovered_img);
}

static void on_key_down(int keycode) {
	//no use
	/*
	switch (keycode) {
	case ALLEGRO_KEY_ENTER:
		game_change_scene(scene_main_create());
		break;
	default:
		break;
	}
	*/
}

// The only function that is shared across files.
Scene scene_settings_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	btn2 = button_create(730, 20, 50, 50, "Assets/strawberry.png", "Assets/grape.png");
	mute = button_create(SCREEN_W / 2 - 200, SCREEN_H / 2, 50, 50, "Assets/strawberry.png", "Assets/grape.png");
	unmute = button_create(SCREEN_W / 2 + 145, SCREEN_H / 2, 50, 50, "Assets/strawberry.png", "Assets/grape.png");

	scene.name = "Settings";
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_move = &on_mouse_move;
	scene.on_mouse_down = &on_mouse_down;

	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Settings scene created");
	return scene;
}
