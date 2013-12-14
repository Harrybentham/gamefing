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
	int wallno=0;
	int min[6]={0,0,0,0,120,240};
	int max[6]={0,240,300,120,240,420};
	int x=0,y=0;
	int spawnx=0;
	int spawny=0;

	//wall move status 0=left or down, 1=right or up
	int move[6] ={0,0,0,0,0,0};
	
	int wallw[]={0,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60};
	int wallh[]={0,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60};
	int wallx[]={0,240,360,576,480,240,0,60,180,180,300,300,420,420,420,420,480,576,60,120,180,240,300,0,60,120,180,180,180,300,300,360,420,480};
	int wally[]={0,120,0,0,120,240,60,60,0,60,60,120,60,120,180,240,60,180,180,180,180,180,240,300,300,300,300,360,420,360,420,360,360,360};

	box ship;
	ship.w=60;
	ship.h=60;
	ship.x=0;
	ship.y=0;
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
				for(wallno=0; wallno<sizeof(wallx)/sizeof(int); wallno++){
					if(i>wallx[wallno] && i<wallx[wallno] + wallw[wallno] && o>wally[wallno] && o<wally[wallno] + wallh[wallno]){
						ship.y=spawny;
						ship.x=spawnx;	
						}
						else{
						//do nothing;
						}
					}
				}
			}

		if(wallx[1] <= min[1]){
		move[1]=1;
		}
		
		if(wallx[1] >= max[1]){
		move[1]=0;
		}

		if(move[1] == 0){
		wallx[1]-=2;
		}

		if(move[1] == 1){
		wallx[1]+=2;
		}
		


	
		for(wallno=2; wallno<6; wallno++){

			if(wally[wallno] <= min[wallno]){
			move[wallno]=1;
			}
		
			if(wally[wallno] >= max[wallno]){
			move[wallno]=0;
			}

			if(move[wallno] == 0){
			wally[wallno]-=2;
			}

			if(move[wallno] == 1){
			wally[wallno]+=2;
			}
		}


		// Apply background to screen
		CCSS_apply_surface(0, 0, background, screen);

		for(wallno=1; wallno<sizeof(wally)/sizeof(int); wallno++){
		//apply walls
		CCSS_apply_surface(wallx[wallno], wally[wallno], wallpic, screen);
		}
		// Apply our character
		CCSS_apply_surface(ship.x, ship.y, character, screen);
		//Apply our flag
		CCSS_apply_surface(580, 420, flag, screen);
		// Built Wall
		CCSS_print(400, 0, font, text_color, screen, "Position %d-%d", ship.x, ship.y);
		//CCSS_print(200, 300, font, text_color, screen, "Wall 1 = %d-%d, move=%d ",wallx[1], wally[1], move[1]); 
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
