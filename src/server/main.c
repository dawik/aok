/*
file:server.c
created:2011-04-13
<wikmans@kth.se>
revisioned by:
*/

#include "init.h"
#include "header.h"
#include "console.h"
#include "net.h"

int main(int argc, char** argv)
{
	struct shared_data d;
	SDL_Thread *console, *sessions, *gameloop, *pathing;
	int player_left; 

	/* Argument check to load map */
	if (argc != 2) {
		fprintf(stdout,"Usage: %s mapname\n",argv[0]);
		exit(1);
	}
	else {
		strcpy(mapname,argv[1]);
		LoadMap(mapname);
	}

	/* Some manual initializations */
	memset(d.client_pool,-1,sizeof(int)*maxclients);
	memset(PLAYER_READY,-1,sizeof(int)*maxclients);
	PACKETSIZE = 1024;
	set_resources(9999);
	reset_dead();
	reset_ready();
	GAME = WARMUP;

	if(Init() == 1) {
		fprintf(stderr, "ERROR: Init()");
		return 1;
	}

	/* Spawn threads */
	console = SDL_CreateThread(consoleThread,&d);
	sessions = SDL_CreateThread(sessionThread,&d);
	gameloop = SDL_CreateThread(Loop,&d);
	pathing = SDL_CreateThread(pathThread,&d);

	while(running) {
		if (number_of_clients < 2) {
			GAME = WARMUP;
		}
		SDL_Delay(1);
		if (ready_check() != 0) {
			start_game(&d);
			set_resources(1000);
			while (GAME == GAME_ON) {
				/* Function returns -1 until winner is decided then the winner ID */
				player_left = decide_winner(d.client_pool,d.clients);
				if (player_left >= 0) {
					send_win(d.clients,player_left);
					reset_ready();
					reset_dead();
					set_resources(9999);
					GAME = WARMUP;
				}
			}
		}
	}
	/* Wait for threads */
	SDL_WaitThread(gameloop, NULL);
	SDL_WaitThread(pathing, NULL);
	SDL_WaitThread(sessions, NULL);
	SDL_WaitThread(console, NULL);
	SDL_Quit();
	return 0;
}
