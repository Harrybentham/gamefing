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
	SDL_Surface* win =NULL;
	SDL_Surface* dead =NULL;
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
	int speed=3;
	int lives =5;
	//maxtime = 5 minutes in millseconds
	int maxtime=300000;
	int time=0;
	int gamestarttick = 0;

	int wallhit =0;
	int flaggamestart=0;

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
	box flagbox;
	flagbox.w=60;
	flagbox.h=60;
	flagbox.x=580;
	flagbox.y=420;
	unsigned char *keystates;
	int starttick, ticks;
	int end = 0;
	int displaywinscreen =0;


	int highscore[]={0,0,0,0,0};
	char *hallofame[5];
	int hallofamecnt=0;
	FILE * hsf, *hsg;
	char playername[3];
	int writecnt=0;
	
	

	printf("Game starting...\n");
	printf("please enter your initials (max 3 characters)\n");
	scanf("%s",&playername);

	screen = CCSS_init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, "super ultra happy mega game!");
	if(screen==NULL){
		printf("Something went wrong!\n");
		exit(-1);
	}





	// Load background image
	background = CCSS_load_image("./resources/img/background.png");
	//Load win screen
	win = CCSS_load_and_resize_image("./resources/img/you_win_this_time.jpg",0.818,0.65);
	//Load dead screen
	dead = CCSS_load_and_resize_image("./resources/img/lol-you-dead-bro-thumb.jpg", 2.10546,1.5789);
	// load character and resize it
	character = CCSS_load_and_resize_image("./resources/img/spaceship.jpg", 0.2, 0.2);
	// load font
	font = TTF_OpenFont( "./resources/font/creaminal.ttf", 14 );	
	// load wallpic
	wallpic = CCSS_load_and_resize_image("./resources/img/wall.jpg", 0.26, 0.26);
	//load flag
	flag = CCSS_load_and_resize_image("./resources/img/flag.jpg", 0.26, 0.26);
	
	gamestarttick = SDL_GetTicks();
	
	while(quit==FALSE && end==0){
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
		if(keystates[SDLK_RETURN])
			end=1;
	
		if(keystates[SDLK_1])
			speed=1;
		if(keystates[SDLK_2])
			speed=2;
		if(keystates[SDLK_3])
			speed=3;
		if(keystates[SDLK_4])
			speed=4;
		if(keystates[SDLK_5])
			speed=5;

		if(ship.x<0){
			ship.x=spawnx;
			ship.y=spawny;
			lives-=1;
			}
		else if(ship.y+ship.h>SCREEN_HEIGHT){
			ship.x=spawnx;
			ship.y=spawny;
			lives-=1;
			}
	
		if(ship.y<0){
			ship.y=spawny;
			ship.x=spawnx;
			lives-=1;
			}
		else if(ship.x+ship.w>SCREEN_WIDTH){
			ship.y=spawny;
			ship.x=spawnx;
			lives-=1;
			}
		
		for(i=ship.x; i<ship.x+ship.w; i++){
			for(o=ship.y; o<ship.y+ship.h; o++){
				for(wallno=0; wallno<sizeof(wallx)/sizeof(int); wallno++){
					if(i>wallx[wallno] && i<wallx[wallno] + wallw[wallno] && o>wally[wallno] && o<wally[wallno] + wallh[wallno]){
						ship.y=spawny;
						ship.x=spawnx;
						wallhit=1;	
						}
						else{
						//do nothing;
						}
					}
				}
			}
		if(wallhit==1){
			lives-=1;
			wallhit=0;
		}
		for(i=ship.x; i<ship.x+ ship.w; i++){
			for(o=ship.y; o<ship.y+ship.h; o++){
				if(i>flagbox.x && i<flagbox.x+flagbox.w && o>flagbox.y && o<flagbox.y + flagbox.h){
					end=1;
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
		wallx[1]-=speed;
		}

		if(move[1] == 1){
		wallx[1]+=speed;
		}
		
		if(lives<=0){
			end=2;
		}


	
		for(wallno=2; wallno<6; wallno++){

			if(wally[wallno] <= min[wallno]){
			move[wallno]=1;
			}
		
			if(wally[wallno] >= max[wallno]){
			move[wallno]=0;
			}

			if(move[wallno] == 0){
			wally[wallno]-=speed;
			}

			if(move[wallno] == 1){
			wally[wallno]+=speed;
			}
		}


		// Apply background to screen
		CCSS_apply_surface(0, 0, background, screen);
		

		

		for(wallno=1; wallno<sizeof(wally)/sizeof(int); wallno++){
		// apply walls
		CCSS_apply_surface(wallx[wallno], wally[wallno], wallpic, screen);
		}

		
		// Apply our character
		CCSS_apply_surface(ship.x, ship.y, character, screen);
		//Apply our flag
		CCSS_apply_surface(580, 420, flag, screen);
		// Built Wall
		//CCSS_print(400, 0, font, text_color, screen, "Position %d-%d", ship.x, ship.y);
		CCSS_print(400, 0, font, text_color, screen, "Lives = %d,",lives); 
		// Update screen
		SDL_Flip( screen );
		ticks = SDL_GetTicks() - starttick;
		if(ticks < 1000 / FRAMES_PER_SECOND){
			SDL_Delay((1000/FRAMES_PER_SECOND)-ticks);
		}
	}
	
	if(end==1){
		printf("ENTERED END 1\n");
		time= SDL_GetTicks() - gamestarttick;
		CCSS_apply_surface(0,0,win, screen);
		CCSS_print(400, 0, font, text_color, screen, "score = %d",lives*speed*time );
		SDL_Flip( screen);
		hsf =fopen("highscore.txt","r");
		for(hallofamecnt=0;hallofamecnt<5 && !feof(hsf);hallofamecnt++){
			printf("ENTERED WHILE LOOP\n");
			fscanf(hsf,"%s %d",&hallofame[hallofamecnt],&highscore[hallofamecnt]);
			printf("%d\n",highscore[hallofamecnt]);
			//printf("%c\n",hallofame[hallofamecnt]);
			if(highscore[hallofamecnt]<lives*speed*time){
				printf("ENTERED IF\n");
				if(!feof(hsf)){
					highscore[hallofamecnt++]=highscore[hallofamecnt];
					hallofame[hallofamecnt++]=hallofame[hallofamecnt];
					hallofamecnt++;
				}
				highscore[hallofamecnt]=lives*speed*time;
				hallofame[hallofamecnt]=playername;
				lives=0;
				
			}

			
			printf("%d\n",hallofamecnt);
		}

		hsg=fopen("highscore.txt","w");		
		for(writecnt=0;writecnt<hallofamecnt;writecnt++){
			printf("Write Count=%d\n",writecnt);
			printf("Score =%d", highscore[writecnt]);		
			fprintf(hsg, "%s %d", hallofame[writecnt], highscore[writecnt]);
			CCSS_print(400,(writecnt+1)*15,font, text_color, screen, "%s = %d", hallofame[writecnt],highscore[writecnt]);
			SDL_Flip(screen);
			}
		
			
			
		SDL_Delay(10000);
	}
	
	if(end==2){
		CCSS_apply_surface(0,0,dead, screen);
		SDL_Flip(screen);
		SDL_Delay(5000);
		SDL_Flip(screen );
	}

	printf("Closing the game...\n");
	// Free background
	SDL_FreeSurface( background );
	SDL_FreeSurface( character );
	SDL_FreeSurface( flag );
	SDL_FreeSurface( wallpic );
	SDL_FreeSurface( win );
	// unload font
	TTF_CloseFont(font);
	fclose(hsf);
	fclose(hsg);
	
	// Quite SDL
	SDL_Quit();

	
	return 0;
}
