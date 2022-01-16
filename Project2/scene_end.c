#define _CRT_SECURE_NO_WARNINGS

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
char score[30];
FILE* pFile = NULL;
int current;
int leader_board[3] = { -1,-1,-1 };

void find(void) {
	pFile = fopen("score_board.txt", "r");

	leader_board[0] = -1;
	leader_board[1] = -1;
	leader_board[2] = -1;

	while (fscanf(pFile, "%d", &current) != EOF) {
		getc(pFile);
		//printf("%d\n", current);
		int tmp;
		for (int i = 0;i < 3;i++) {
			if (current > leader_board[i]) {
				tmp = leader_board[i];
				leader_board[i] = current;
				current = tmp;
			}
		}
	}
}

static void init(void) {
	sprintf_s(score, sizeof(score), "%d", game_main_Score);
	game_log_score(score);
	find();
}

static void draw(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	//char score[30];

	sprintf_s(score, sizeof(score), "Your score is:%4d", game_main_Score);

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2 ,
		SCREEN_H / 2 + 50,
		ALLEGRO_ALIGN_CENTER,
		score
	);

	if (game_win) {
		al_draw_text(
			menuFont,
			al_map_rgb(255, 255, 255),
			SCREEN_W / 2 ,
			SCREEN_H / 2 - 50,
			ALLEGRO_ALIGN_CENTER,
			"YOU WIN!"
		);
	}
	else {
		al_draw_text(
			menuFont,
			al_map_rgb(255, 255, 255),
			SCREEN_W / 2 ,
			SCREEN_H / 2 - 50,
			ALLEGRO_ALIGN_CENTER,
			"YOU LOSE!"
		);
	}

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
	free(pFile);
}

static void on_key_down(int keycode) {
	
}

// The only function that is shared across files.
Scene scene_end_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	okbtn = button_create(730, 20, 50, 50, "Assets/strawberry.png", "Assets/grape.png");

	scene.initialize = &init;
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
