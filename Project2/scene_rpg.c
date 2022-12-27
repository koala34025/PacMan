#define _CRT_SECURE_NO_WARNINGS

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <string.h>
#include "game.h"
#include "shared.h"
#include "utility.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "scene_end.h"
#include "pacman_obj.h"
#include "ghost.h"
#include "map.h"

/* global variables*/
extern const uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;

/* Internal variables*/
static Pacman* pman;
static Map* basic_map;

/* Declare static function prototypes */
static void init(void);
static void step(void);
static void checkItem(void);
static void status_update(void);
static void update(void);
static void draw(void);

static void destroy(void);
static void on_key_down(int key_code);
static void on_mouse_down(void);


static void init(void) {
	
	// create map
	basic_map = create_map("Assets/map_default.txt");

	if (!basic_map) {
		game_abort("error on creating map");
	}	
	// create pacman
	pman = pacman_create();
	if (!pman) {
		game_abort("error on creating pacamn\n");
	}
	
	// allocate ghost memory
	// [HACKATHON 2-1]
	// TODO: Allocate dynamic memory for ghosts array.
	/*
	ghosts = (...)malloc(sizeof(...) * GHOST_NUM)
	*/

	GAME_TICK = 0;

	return ;
}

static void step(void) {
	if (pman->objData.moveCD > 0)
		pman->objData.moveCD -= pman->speed;

}
static void checkItem(void) {
	int Grid_x = pman->objData.Coord.x, Grid_y = pman->objData.Coord.y;
	if (Grid_y >= basic_map->row_num - 1 || Grid_y <= 0 || Grid_x >= basic_map->col_num - 1 || Grid_x <= 0)
		return;
	// [HACKATHON 1-3]
	// TODO: check which item you are going to eat and use `pacman_eatItem` to deal with it.

	switch (basic_map->map[Grid_y][Grid_x])
	{
	default:
		pacman_eatItem(pman, '.');
		break;
	}
}
	
static void status_update(void) {

	
		//RecArea RA = getDrawArea(pman->objData, GAME_TICK_CD);
		//RecArea RB = getDrawArea(ghosts[i]->objData, GAME_TICK_CD);
		
		//if (RecAreaOverlap(RA, RB)){
			
		//}



}

static void update(void) {
	if (0) {
		
		return;
	}

	step();
	checkItem();
	status_update();
	pacman_move(pman, basic_map);
	
}

static void draw(void) {

	al_clear_to_color(al_map_rgb(0, 0, 0));

	draw_map(basic_map);

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2 - 80,
		SCREEN_H - 100,
		ALLEGRO_ALIGN_LEFT,
		"HI"
	);

	pacman_draw(pman);

}


static void destroy(void) {
	/*
		[TODO]
		free map array, Pacman and ghosts
	*/
	
	pacman_destroy(pman);
	
	delete_map(basic_map);
	//free(basic_map);
	
}

static void on_key_down(int key_code) {
	switch (key_code)
	{
		// [HACKATHON 1-1]	
		// TODO: Use allegro pre-defined enum ALLEGRO_KEY_<KEYNAME> to controll pacman movement
		// we provided you a function `pacman_NextMove` to set the pacman's next move direction.
		
		case ALLEGRO_KEY_W:
			pacman_NextMove(pman, UP);
			break;
		case ALLEGRO_KEY_A:
			pacman_NextMove(pman, LEFT);
			break;
		case ALLEGRO_KEY_S:
			pacman_NextMove(pman, DOWN);
			break;
		case ALLEGRO_KEY_D:
			pacman_NextMove(pman, RIGHT);
			break;
		case ALLEGRO_KEY_ENTER:
			game_change_scene(scene_main_create());
			break;
	default:
		break;
	}

}


static void on_mouse_down(void) {
	// nothing here

}

Scene scene_rpg_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "RPG";
	scene.initialize = &init;
	scene.update = &update;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_down = &on_mouse_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("RPG scene created");
	return scene;
}
