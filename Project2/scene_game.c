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
#define BONUS 2

// [HACKATHON 2-0]
// Just modify the GHOST_NUM to 1
#define GHOST_NUM 4
/* global variables*/
extern const uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
extern int game_main_Score = 0;
int game_bean_Score = 0;
int game_ghost_Score = 0;
extern bool game_over = false;
extern bool game_win = false;
extern ALLEGRO_SAMPLE* PACMAN_EATGHOST_SOUND;

/* Internal variables*/
static ALLEGRO_TIMER* power_up_timer;
static const int power_up_duration = 10;
static Pacman* pman;
static Map* basic_map;
static Ghost** ghosts;
bool debug_mode = false;
bool cheat_mode = false;
static ALLEGRO_SAMPLE_ID PACMAN_EATGHOST_SOUND_ID;
static ALLEGRO_TIMER* item1_timer;
static const int item1_duration = 10;
static ALLEGRO_TIMER* item2_timer;
static const int item2_duration = 10;

/* Declare static function prototypes */
static void init(void);
static void step(void);
static void checkItem(void);
static void status_update(void);
static void update(void);
static void draw(void);
static void printinfo(void);
static void destroy(void);
static void on_key_down(int key_code);
static void on_mouse_down(void);
static void render_init_screen(void);
static void draw_hitboxes(void);

static void init(void) {
	game_over = false;
	game_win = false;
	game_main_Score = 0;
	game_bean_Score = 0;
	game_ghost_Score = 0;
	// create map
	basic_map = create_map("Assets/map_nthu.txt");//hightlight nthu;

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
	ghosts = (Ghost**)malloc(sizeof(Ghost*) * GHOST_NUM);
	if(!ghosts){
		game_log("We haven't create any ghosts!\n");
	}
	else {
		// [HACKATHON 2-2]
		// TODO: create a ghost.
		// Try to look the definition of ghost_create and figure out what should be placed here.
		for (int i = 0; i < GHOST_NUM; i++) {
			
			game_log("creating ghost %d\n", i);
			ghosts[i] = ghost_create(i);  
			if (!ghosts[i])
				game_abort("error creating ghost\n");
			
		}
	}
	GAME_TICK = 0;

	render_init_screen();
	power_up_timer = al_create_timer(1.0f); // 1 tick / sec
	if (!power_up_timer)
		game_abort("Error on create timer of power\n");
	item1_timer = al_create_timer(1.0f); // 1 tick / sec
	if (!item1_timer)
		game_abort("Error on create timer of item1\n");
	item2_timer = al_create_timer(1.0f); // 1 tick / sec
	if (!item2_timer)
		game_abort("Error on create timer of item2\n");
	return ;
}

static void step(void) {
	if (pman->objData.moveCD > 0)
		pman->objData.moveCD -= pman->speed;
	for (int i = 0; i < GHOST_NUM; i++) {
		// important for movement
		if (ghosts[i]->objData.moveCD > 0)
			ghosts[i]->objData.moveCD -= ghosts[i]->speed;
	}
}
static void checkItem(void) {
	int Grid_x = pman->objData.Coord.x, Grid_y = pman->objData.Coord.y;
	if (Grid_y >= basic_map->row_num - 1 || Grid_y <= 0 || Grid_x >= basic_map->col_num - 1 || Grid_x <= 0)
		return;
	// [HACKATHON 1-3]
	// TODO: check which item you are going to eat and use `pacman_eatItem` to deal with it.

	switch (basic_map->map[Grid_y][Grid_x])
	{
	case '.':
		basic_map->map[Grid_y][Grid_x] = ' ';

		basic_map->beansCount--;
		if (pman->powerUp) basic_map->score += 1 * BONUS;
		else basic_map->score += 1;
		pacman_eatItem(pman, '.');
		break;
	case 'P':
		basic_map->map[Grid_y][Grid_x] = ' ';
		for (int i = 0;i < GHOST_NUM; i++) {
			ghost_toggle_FLEE(ghosts[i], true);
		}
		al_set_timer_count(power_up_timer, 0);
		al_start_timer(power_up_timer);
		pacman_eatItem(pman, 'P');
		break;
	case 'X':
		basic_map->map[Grid_y][Grid_x] = ' ';
		//something to do with item 1
		pman->powerUp = true;
		al_set_timer_count(item1_timer, 0);
		al_start_timer(item1_timer);
		pacman_eatItem(pman, 'X');
		break;
	case 'Y':
		basic_map->map[Grid_y][Grid_x] = ' ';
		//something to do with item 2
		for (int i = 0;i < GHOST_NUM; i++) {
			ghost_toggle_CRAZE(ghosts[i], true);
		}
		al_set_timer_count(item2_timer, 0);
		al_start_timer(item2_timer);
		pacman_eatItem(pman, 'Y');
		break;
	default:
		break;
	}
}
	
static void status_update(void) {
	if (al_get_timer_count(power_up_timer) >= power_up_duration - 2 && al_get_timer_count(power_up_timer) < power_up_duration) {
		for (int i = 0;i < GHOST_NUM;i++) {
			if (ghosts[i]->status == FLEE) {
				ghosts[i]->status = preFREEDOM;
			}
		}
	}

	if (al_get_timer_count(power_up_timer) >= power_up_duration ) {
		for (int i = 0;i < GHOST_NUM;i++) {
			ghost_toggle_FLEE(ghosts[i], false);
		}
		al_stop_timer(power_up_timer);
		al_set_timer_count(power_up_timer, 0);
	}

	if (al_get_timer_count(item1_timer) >= item1_duration) {
		//something to end with item 1
		pman->powerUp = false;
		al_stop_timer(item1_timer);
		al_set_timer_count(item1_timer, 0);
	}

	if (al_get_timer_count(item2_timer) >= item2_duration) {
		//something to end with item 2
		for (int i = 0;i < GHOST_NUM; i++) {
			ghost_toggle_CRAZE(ghosts[i], false);
		}
		al_stop_timer(item2_timer);
		al_set_timer_count(item2_timer, 0);
	}
	for (int i = 0; i < GHOST_NUM; i++) {		
		if (ghosts[i]->status == GO_IN)
			continue;
		
		RecArea RA = getDrawArea(pman->objData, GAME_TICK_CD);
		RecArea RB = getDrawArea(ghosts[i]->objData, GAME_TICK_CD);
		
		if (!cheat_mode && RecAreaOverlap(RA, RB)){
			if (ghosts[i]->status == FREEDOM || ghosts[i]->status == CRAZE) {
				game_log("collide with ghost %d and die\n",i);
				al_rest(1.0);
				pacman_die();
				game_over = true;
				break;
			}
			else if (ghosts[i]->status == FLEE || ghosts[i]->status == preFREEDOM) {
				game_log("collide with ghost %d and eat it\n", i);
				stop_bgm(PACMAN_EATGHOST_SOUND_ID);
				PACMAN_EATGHOST_SOUND_ID = play_audio(PACMAN_EATGHOST_SOUND, effect_volume);
				if (pman->powerUp) game_ghost_Score += 3 * BONUS;
				else game_ghost_Score += 3;
				ghost_collided(ghosts[i]);
			}
		}
	}

	if (cheat_mode) {
		for (int i = 0;i < GHOST_NUM; i++) {
			ghost_toggle_FLEE(ghosts[i], true);
		}
		al_set_timer_count(power_up_timer, 0);
		al_start_timer(power_up_timer);
		pacman_eatItem(pman, 'P');

		/*
		al_set_timer_count(item1_timer, 0);
		al_start_timer(item1_timer);
		pacman_eatItem(pman, 'X');
		*/
		cheat_mode = false;
	}

}

static void update(void) {
	if (game_win) {
		game_change_scene(scene_end_create());
		//game_change_scene(scene_menu_create());	
		return;
	}

	if (game_over) {
		/*
			[TODO]
			start pman->death_anim_counter and schedule a game-over event (e.g change scene to menu) after Pacman's death animation finished
			game_change_scene(...);
		*/
		al_start_timer(pman->death_anim_counter);

		//write a thorough game_over_event() functiono;

		if (al_get_timer_count(pman->death_anim_counter) > 192) {
			al_stop_timer(pman->death_anim_counter);
			al_set_timer_count(pman->death_anim_counter, 0);
			game_change_scene(scene_end_create());
			//game_change_scene(scene_menu_create());
		}
		return;
	}

	step();
	checkItem();
	status_update();
	pacman_move(pman, basic_map);
	for (int i = 0; i < GHOST_NUM; i++) {
		ghosts[i]->move_script(ghosts[i], basic_map, pman);
	}

}

static void draw(void) {

	al_clear_to_color(al_map_rgb(0, 0, 0));
	
	//	[TODO]
	//	Draw scoreboard, something your may need is sprinf();
	/*
		al_draw_text(...);
	*/

	draw_map(basic_map);

	char score[30];

	game_bean_Score = basic_map->score;

	game_main_Score = game_bean_Score + game_ghost_Score;
	sprintf_s(score, sizeof(score), "SCORE:%4d", game_main_Score);//GAME_MAIN_SCORE is availible

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2 - 80,
		SCREEN_H - 100,
		ALLEGRO_ALIGN_LEFT,
		score
	);

	pacman_draw(pman);

	if (pman->powerUp) {
		al_draw_text(
			menuFont,
			al_map_rgb(255, 215, 0),
			SCREEN_W - 200,
			SCREEN_H - 100,
			ALLEGRO_ALIGN_CENTER,
			"!! DOUBLE !!"
		);
		al_draw_text(
			menuFont,
			al_map_rgb(255, 215, 0),
			200,
			SCREEN_H - 100,
			ALLEGRO_ALIGN_CENTER,
			"!! DOUBLE !!"
		);
	}

	if (game_win) return;
	if (game_over) return;
	if (basic_map->beansCount == 0) {
		game_log("no beans left\n");
		al_rest(1.0);
		game_win = true;
	}

	// no drawing below when game over
	for (int i = 0; i < GHOST_NUM; i++)
		ghost_draw(ghosts[i]);
	
	//debugging mode
	if (debug_mode) {
		draw_hitboxes();
	}

}

static void draw_hitboxes(void) {
	RecArea pmanHB = getDrawArea(pman->objData, GAME_TICK_CD);
	al_draw_rectangle(
		pmanHB.x, pmanHB.y,
		pmanHB.x + pmanHB.w, pmanHB.y + pmanHB.h,
		al_map_rgb_f(1.0, 0.0, 0.0), 2
	);

	for (int i = 0; i < GHOST_NUM; i++) {
		RecArea ghostHB = getDrawArea(ghosts[i]->objData, GAME_TICK_CD);
		al_draw_rectangle(
			ghostHB.x, ghostHB.y,
			ghostHB.x + ghostHB.w, ghostHB.y + ghostHB.h,
			al_map_rgb_f(1.0, 0.0, 0.0), 2
		);
	}

}

static void printinfo(void) {
	game_log("pacman:\n");
	game_log("coord: %d, %d\n", pman->objData.Coord.x, pman->objData.Coord.y);
	game_log("PreMove: %d\n", pman->objData.preMove);
	game_log("NextTryMove: %d\n", pman->objData.nextTryMove);
	game_log("Speed: %f\n", pman->speed);
}


static void destroy(void) {
	/*
		[TODO]
		free map array, Pacman and ghosts
	*/
	
	pacman_destroy(pman);
	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_destroy(ghosts[i]);
	}
	delete_map(basic_map);
	//free(basic_map);
	free(ghosts);
	al_destroy_timer(power_up_timer);
	al_destroy_timer(item1_timer);
	al_destroy_timer(item2_timer);
	stop_bgm(PACMAN_EATGHOST_SOUND_ID);
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
		case ALLEGRO_KEY_C:
			cheat_mode = !cheat_mode;
			if (cheat_mode)
				printf("cheat mode on\n");
			else 
				printf("cheat mode off\n");
			break;
		case ALLEGRO_KEY_G:
			debug_mode = !debug_mode;
			break;
		
	default:
		break;
	}

}

static void on_mouse_down(void) {
	// nothing here

}

static void render_init_screen(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	draw_map(basic_map);
	pacman_draw(pman);
	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_draw(ghosts[i]);
	}

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		400, 400,
		ALLEGRO_ALIGN_CENTER,
		"READY!"
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2 - 75,
		SCREEN_H - 100,
		ALLEGRO_ALIGN_LEFT,
		"SCORE: "
	);

	al_flip_display();
	al_rest(2.0);

}
// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_main_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Start";
	scene.initialize = &init;
	scene.update = &update;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_down = &on_mouse_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Start scene created");
	return scene;
}
