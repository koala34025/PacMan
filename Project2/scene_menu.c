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

bool isYellow = true;
extern int game_main_Score;
extern int leader_board[3];

/* Internal Variables*/
static ALLEGRO_BITMAP* gameTitle = NULL;
static ALLEGRO_SAMPLE_ID menuBGM;
static int gameTitleW ;
static int gameTitleH ;

// [HACKATHON 3]
// TARGET : use a clickable button to enter setting scene.
// For `Button` struct(object) is defined in `scene_menu_object.h` and `scene_menu_object.c`
// STRONGLY recommend you trace both of them first. 

//	[HACKATHON 3-1]
//	TODO: Declare variable for button
//	Uncomment and fill the code below
// static ... btnSettings;

static Button btnSettings;
static Button chooseYellow;
static Button chooseBrown;
static Button chosenYellow;
static Button chosenBrown;

static void init() {
	static int first_time = true;
	if (first_time) {
		find();
		first_time = false;
	}
	/*
	int current = game_main_Score, tmp;
	for (int i = 0;i < 3;i++) {
		if (current > leader_board[i]) {
			tmp = leader_board[i];
			leader_board[i] = current;
			current = tmp;
		}
	}
	game_main_Score = 0;
	*/


	// [HACKATHON 3-2]
	// TODO: Create button to settings
	//	Uncomment and fill the code below
	//btnSettings = button_create(730, 20, 50, 50, "...", "...");
	
	btnSettings = button_create(730, 20, 50, 50, "Assets/settings.png", "Assets/settings2.png");
	chooseYellow = button_create(SCREEN_W / 2 - 160, SCREEN_H / 2 + 100, 24, 24, "Assets/o.png", "Assets/o.png");
	chooseBrown = button_create(SCREEN_W / 2 + 40, SCREEN_H / 2 + 100, 24, 24, "Assets/o.png", "Assets/o.png");
	chosenYellow = button_create(SCREEN_W / 2 - 160, SCREEN_H / 2 + 100, 24, 24, "Assets/dot.png", "Assets/dot.png");
	chosenBrown = button_create(SCREEN_W / 2 + 40, SCREEN_H / 2 + 100, 24, 24, "Assets/dot.png", "Assets/dot.png");

	gameTitle = load_bitmap("Assets/title.png");
	gameTitleW = al_get_bitmap_width(gameTitle);
	gameTitleH = al_get_bitmap_height(gameTitle);


	stop_bgm(menuBGM);
	menuBGM = play_bgm(themeMusic, music_volume);
	
}


static void draw() {

	al_clear_to_color(al_map_rgb(0, 0, 0));

	const float scale = 0.7;
	const float offset_w = (SCREEN_W >> 1) - 0.5 * scale * gameTitleW;
	const float offset_h = (SCREEN_H >> 1) - 0.5 * scale * gameTitleH;

	//draw title image
	al_draw_scaled_bitmap(
		gameTitle,
		0, 0,
		gameTitleW, gameTitleH,
		offset_w, offset_h,
		gameTitleW * scale, gameTitleH * scale,
		0
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W/2,
		SCREEN_H - 150,
		ALLEGRO_ALIGN_CENTER,
		"PRESS \"ENTER\""
	);
	al_draw_text(
		menuFont,
		al_map_rgb(185, 122, 86),
		SCREEN_W / 2 + 115,
		SCREEN_H / 2 + 100,
		ALLEGRO_ALIGN_CENTER,
		"Brown"
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		SCREEN_W / 2 - 85,
		SCREEN_H / 2 + 100,
		ALLEGRO_ALIGN_CENTER,
		"Yellow"
	);
	
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		0,
		0,
		ALLEGRO_ALIGN_LEFT,
		"Leader Board"
	);
	
	char score[30];
	sprintf_s(score, sizeof(score), "Gold:%4d", leader_board[0]);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 215, 0),
		0,
		0 + 35,
		ALLEGRO_ALIGN_LEFT,
		score
	);

	sprintf_s(score, sizeof(score), "Silver:%4d", leader_board[1]);
	al_draw_text(
		menuFont,
		al_map_rgb(192, 192, 192),
		0,
		0 + 70,
		ALLEGRO_ALIGN_LEFT,
		score
	);

	sprintf_s(score, sizeof(score), "Bronze:%4d", leader_board[2]);
	al_draw_text(
		menuFont,
		al_map_rgb(205, 127, 50),
		0,
		0 + 105,
		ALLEGRO_ALIGN_LEFT,
		score
	);

		// [HACKATHON 3-3]
		// TODO: Draw button
		// Uncomment and fill the code below
		// drawButton(...);
	drawButton(btnSettings);
	if(isYellow) drawButton(chosenYellow);
	else drawButton(chosenBrown);
	drawButton(chooseYellow);
	drawButton(chooseBrown);
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	//	[HACKATHON 3-7]
	//	TODO: Update button's status(hovered), and utilize the function `pnt_in_rect`, which you just implemented
	//	Uncomment and fill the code below
	//	 btnSettings.hovered = ???(btnSettings, mouse_x, mouse_y);
	btnSettings.hovered = buttonHover(btnSettings, mouse_x, mouse_y);
	chooseYellow.hovered = buttonHover(chooseYellow, mouse_x, mouse_y);
	chooseBrown.hovered = buttonHover(chooseBrown, mouse_x, mouse_y);
	chosenYellow.hovered = buttonHover(chosenYellow, mouse_x, mouse_y);
	chosenBrown.hovered = buttonHover(chosenBrown, mouse_x, mouse_y);
}


//	[HACKATHON 3-8]
//	TODO: When btnSettings clicked, switch to settings scene
//  `game_change_scene` is defined in `game.h`.
//  You can check line 121 `scene_menu.c` to see how to use this function.
//  And find the corresponding function for `scene_setting`
// 	Utilize the member defined in struct `Button`.
//  The logic here is 
//  `if clicked the mouse` && `mouse position is in the area of button`
// 	      `Enter the setting scene`
//	Uncomment and fill the code below
/*
static void on_mouse_down() {
	if (btnSettings.???)
		game_change_scene(...);
}
*/
static void on_mouse_down() {
	if (btnSettings.hovered)
		game_change_scene(scene_settings_create());
	if (chooseYellow.hovered) {
		isYellow = true;
		game_log("choose yellow pacman");
	}
	if (chooseBrown.hovered) {
		isYellow = false;
		game_log("choose brown pacman");
	}
}

static void destroy() {
	stop_bgm(menuBGM);
	al_destroy_bitmap(gameTitle);

	al_destroy_bitmap(btnSettings.default_img);
	al_destroy_bitmap(btnSettings.hovered_img);
	al_destroy_bitmap(chooseYellow.default_img);
	al_destroy_bitmap(chooseYellow.hovered_img);
	al_destroy_bitmap(chooseBrown.default_img);
	al_destroy_bitmap(chooseBrown.hovered_img);
	al_destroy_bitmap(chosenYellow.default_img);
	al_destroy_bitmap(chosenYellow.hovered_img);
	al_destroy_bitmap(chosenBrown.default_img);
	al_destroy_bitmap(chosenBrown.hovered_img);
}

static void on_key_down(int keycode) {

	switch (keycode) {
		case ALLEGRO_KEY_ENTER:
			game_change_scene(scene_main_create());
			break;
		default:
			break;
	}
}

// TODO: Add more event callback functions such as update, ...

// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_menu_create(void) {

	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Menu";
	scene.initialize = &init;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_move = &on_mouse_move;
	// [HACKATHON 3-9]
	// TODO: Register on_mouse_down.
	// Uncomment the code below.
	/*
	scene.on_mouse_down = &on_mouse_down;
	*/
	// -------------------------------------
	scene.on_mouse_down = &on_mouse_down;


	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Menu scene created");
	return scene;
}