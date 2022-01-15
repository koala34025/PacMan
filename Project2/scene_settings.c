#include <stdlib.h>
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
static Button music_up;
static Button music_down;
static Button effect_up;
static Button effect_down;
static ALLEGRO_SAMPLE_ID settingsBGM;

static void init() {
	btn2 = button_create(730, 20, 50, 50, "Assets/x.png", "Assets/x2.png");
	music_up = button_create(SCREEN_W / 2 + 125, SCREEN_H / 2 - 65, 50, 50, "Assets/plus.png", "Assets/plus2.png");
	music_down = button_create(SCREEN_W / 2 - 180, SCREEN_H / 2 - 65, 50, 50, "Assets/minus.png", "Assets/minus2.png");
	effect_up = button_create(SCREEN_W / 2 + 125, SCREEN_H / 2 + 35, 50, 50, "Assets/plus.png", "Assets/plus2.png");
	effect_down = button_create(SCREEN_W / 2 - 180, SCREEN_H / 2 + 35, 50, 50, "Assets/minus.png", "Assets/minus2.png");
	stop_bgm(settingsBGM);
	settingsBGM = play_bgm(settingsMusic, music_volume);
}

static void play_BGM(void) {
	stop_bgm(settingsBGM);
	settingsBGM = play_bgm(settingsMusic, music_volume);
}

static void draw(void){
	al_clear_to_color(al_map_rgb(0, 0, 0));

	drawButton(btn2);
	drawButton(music_up);
	drawButton(music_down);
	drawButton(effect_up);
	drawButton(effect_down);

	char m_volume[100];
	sprintf_s(m_volume, sizeof(m_volume), "Music:%7.f%%", music_volume < 0.02 ? (double)0 : music_volume * 100 / 0.5);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2 - 100,
		SCREEN_H / 2 - 50,
		ALLEGRO_ALIGN_LEFT,
		m_volume
	);

	char e_volume[100];
	sprintf_s(e_volume, sizeof(e_volume), "Effect:%5.f%%", effect_volume < 0.02 ? (double)0 : effect_volume * 100 / 0.5);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2 - 100,
		SCREEN_H / 2 + 50,
		ALLEGRO_ALIGN_LEFT,
		e_volume
	);
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	btn2.hovered = buttonHover(btn2, mouse_x, mouse_y);
	music_up.hovered = buttonHover(music_up, mouse_x, mouse_y);
	music_down.hovered = buttonHover(music_down, mouse_x, mouse_y);
	effect_up.hovered = buttonHover(effect_up, mouse_x, mouse_y);
	effect_down.hovered = buttonHover(effect_down, mouse_x, mouse_y);
}

static void on_mouse_down() {
	if (btn2.hovered)
		game_change_scene(scene_menu_create());
	if (music_up.hovered && music_volume < 0.5) {
		music_volume += 0.025;
		play_BGM();
		printf("%f\n", music_volume);
	}
	if (music_down.hovered && music_volume > 0) {
		music_volume -= 0.025;
		play_BGM();
		printf("%f\n", music_volume);
	}
	if (effect_up.hovered && effect_volume < 0.5) {
		effect_volume += 0.025;
		play_BGM();
		printf("%f\n", effect_volume);
	}
	if (effect_down.hovered && effect_volume > 0) {
		effect_volume -= 0.025;
		play_BGM();
		printf("%f\n", effect_volume);
	}
}

static void destroy() {
	stop_bgm(settingsBGM);
	al_destroy_bitmap(btn2.default_img);
	al_destroy_bitmap(btn2.hovered_img);
	al_destroy_bitmap(music_up.default_img);
	al_destroy_bitmap(music_up.hovered_img);
	al_destroy_bitmap(music_down.default_img);
	al_destroy_bitmap(music_down.hovered_img);
	al_destroy_bitmap(effect_up.default_img);
	al_destroy_bitmap(effect_up.hovered_img);
	al_destroy_bitmap(effect_down.default_img);
	al_destroy_bitmap(effect_down.hovered_img);
}

static void on_key_down(int keycode) {

}


// The only function that is shared across files.
Scene scene_settings_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));

	scene.name = "Settings";
	scene.initialize = &init;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_move = &on_mouse_move;
	scene.on_mouse_down = &on_mouse_down;

	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Settings scene created");
	return scene;
}
