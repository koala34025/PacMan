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
#include "scene_end.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "utility.h"
#include "shared.h"

static Button okbtn;
extern int game_main_Score;
extern bool game_over;
extern bool game_win;

static void draw(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	char score[100];

	sprintf_s(score, sizeof(score), "SCORE:%4d", game_main_Score);

	if (game_win) {
		al_draw_text(
			menuFont,
			al_map_rgb(255, 255, 255),
			SCREEN_W / 2 - 75,
			SCREEN_H - 200,
			ALLEGRO_ALIGN_LEFT,
			"WIN"
		);
	}
	else {
		al_draw_text(
			menuFont,
			al_map_rgb(255, 255, 255),
			SCREEN_W / 2 - 75,
			SCREEN_H - 200,
			ALLEGRO_ALIGN_LEFT,
			"LOSE"
		);
	}

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2 - 75,
		SCREEN_H - 100,
		ALLEGRO_ALIGN_LEFT,
		score
	);

	drawButton(okbtn);
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	okbtn.hovered = buttonHover(okbtn, mouse_x, mouse_y);
}

static void on_mouse_down() {
	if (okbtn.hovered)
		game_change_scene(scene_menu_create());
}

static void destroy() {
	al_destroy_bitmap(okbtn.default_img);
	al_destroy_bitmap(okbtn.hovered_img);
}

static void on_key_down(int keycode) {
	
}

// The only function that is shared across files.
Scene scene_end_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	okbtn = button_create(730, 20, 50, 50, "Assets/strawberry.png", "Assets/grape.png");

	scene.name = "End";
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_move = &on_mouse_move;
	scene.on_mouse_down = &on_mouse_down;

	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("End scene created");
	return scene;
}
