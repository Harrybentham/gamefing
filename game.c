#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "./lib/ccss/ccss.h"

#define SCREEN_WIDTH 		640
#define SCREEN_HEIGHT 		480
#define SCREEN_BPP 		32

#define FRAMES_PER_SECOND	24

#define BOOL 			unsigned char
#define TRUE 			1
#define FALSE 			0

#define WHITE		{255, 255, 255}

typedef struct{
	int x;
	int y;
	int w;
	int h;
} box;

int main( void ){
	BOOL quit = FALSE;
	// surfaces
	SDL_Surface* screen = NULL;
	SDL_Surface* background = NULL;
	SDL_Surface* wallpic = NULL;
	SDL_Surface* character = NULL;
	SDL_Surface* flag = NULL;
	SDL_Event event;
	// font
	TTF_Font* font = NULL;
	SDL_Color text_color = WHITE;
	// game data
	int i=0;
	int o=0;
	int p=0;
	int u=0;
	int x=25,y=25;
	int spawnx=25;
	int spawny=25;
	int move = 1;
	box wall;
	wall.w=60;
	wall.h=60;
	wall.x=220;
	wall.y=220;
	box mwall;
	mwall.w=60;
	mwall.h=60;
	mwall.x=450;
	mwall.y=150;
	box ship;
	ship.w=60;
	ship.h=60;
	ship.x=25;
	ship.y=25;
	unsigned char *keystates;
	int starttick, ticks;

	printf("Game starting...\n");

	screen = CCSS_init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, "super ultra happy mega game!");
	if(screen==NULL){
		printf("Something went wrong!\n");
		exit(-1);
	}

	// Load background image
	background = CCSS_load_image("./resources/img/background.png");
	// load character and resize it
	character = CCSS_load_and_resize_image("./resources/img/spaceship.jpg", 0.2, 0.2);
	// load font
	font = TTF_OpenFont( "./resources/font/creaminal.ttf", 14 );	
	// load wallpic
	wallpic = CCSS_load_and_resize_image("./resources/img/wall.jpg", 0.26, 0.26);
	//load flag
	flag = CCSS_load_and_resize_image("./resources/img/flag.jpg", 0.2, 0.2);

	while(quit==FALSE){
		starttick = SDL_GetTicks();
		while(SDL_PollEvent( &event )){ // while there is event to handle
			if(event.type == SDL_QUIT){
				quit = TRUE;
			}
		}
		keystates = SDL_GetKeyState( NULL );
		if(keystates[SDLK_UP])
			ship.y-=5;
		if(keystates[SDLK_DOWN])
			ship.y+=5;
		if(keystates[SDLK_LEFT])
			ship.x-=5;
		if(keystates[SDLK_RIGHT])
			ship.x+=5;



		if(ship.x<0){
			ship.x=spawnx;
			ship.y=spawny;
			}
		else if(ship.y+ship.h>SCREEN_HEIGHT){
			ship.x=spawnx;
			ship.y=spawny;
			}
	
		if(ship.y<0){
			ship.y=spawny;
			ship.x=spawnx;
			}
		else if(ship.x+ship.w>SCREEN_WIDTH){
			ship.y=spawny;
			ship.x=spawnx;
			}

		for(i=ship.x; i<ship.x+ship.w; i++){
			for(o=ship.y; o<ship.y+ship.h; o++){
				if(i>wall.x && i<wall.x + wall.w && o>wall.x && o<wall.y + wall.h){
			ship.y=spawny;
			ship.x=spawnx;	
			}
			else{
			//do nothing;
		}
		}
		}

		for(p=ship.x; p<ship.x+ship.w; p++){
			for(u=ship.y; u<ship.y+ship.h; u++){
				if(p>mwall.x && p<mwall.x + mwall.w && u>mwall.x && u<mwall.y + mwall.h){
			ship.y=spawny;
			ship.x=spawnx;	
			}
			else{
			//do nothing;

		}
		}
		}
		
		if(mwall.y == 150){
		move=1;
		}
		
		if(mwall.y == 300){
		move=0;
		}

		if(move == 0){
		mwall.y-=5;
		}

		if(move == 1){
		mwall.y+=5;
		}


		// Apply background to screen
		CCSS_apply_surface(0, 0, background, screen);		
		// Apply our character
		CCSS_apply_surface(ship.x, ship.y, character, screen);
		//apply moving wall
		CCSS_apply_surface(mwall.x, mwall.y, wallpic, screen);
		//Apply our flag
		CCSS_apply_surface(580, 420, flag, screen);
		// We print something
		CCSS_apply_surface(wall.x, wall.y, wallpic, screen);
		// Built Wall
		CCSS_print(400, 0, font, text_color, screen, "Position %d-%d", ship.x, ship.y); 
		// Update screen
		SDL_Flip( screen );
		ticks = SDL_GetTicks() - starttick;
		if(ticks < 1000 / FRAMES_PER_SECOND){
			SDL_Delay((1000/FRAMES_PER_SECOND)-ticks);
		}
	}

	printf("Closing the game...\n");
	// Free background
	SDL_FreeSurface( background );
	SDL_FreeSurface( character );
	SDL_FreeSurface( flag );
	SDL_FreeSurface( wallpic );
	// unload font
	TTF_CloseFont(font);
	
	// Quite SDL
	SDL_Quit();

	
	return 0;
}
