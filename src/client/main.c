/*
 * File:   main.c
 * Author: Projektgrupp 4
 * Created by: Tobias Hultgren
 * Revisioned by:
 *
 * Created on April 2, 2011
 */

#include "header.h"
#include "menu.h"
#include "net.h"

int main(int argc, char** argv)
{
    //freopen("CON", "w", stdout); //Makes it possible to read console information under Windows

	struct shared_data d;
	IPaddress server;
	/* Init */
	if(Init() == 1) {
		fprintf(stderr, "ERROR: Init()");
		return 1;
	}

	glutInit(&argc, argv);

	SDL_Thread* threadRender = SDL_CreateThread(Render, NULL); //Creates rendering thread
	if(threadRender==NULL) {
		fprintf(stderr,"Error: threadRender\n");
		return -1;
	}

	SDL_Delay(150); //Short wait for Linux compatibility

    startMenuOpen = true;
	SDL_Event EventData;
	while(startMenuOpen) { //main threads handles event polling
		while (SDL_PollEvent(&EventData))
		{
			Event(&EventData);
		}
		SDL_Delay(1);
	}

    if(running)
    {
        strcpy(d.name,settings.username);
        /* Connect to remote server */
        if ((SDLNet_ResolveHost(&server, settings.server, (Uint16)settings.port))<0) {
            fprintf(stderr,"Cant resolve server address\n");
            exit(2);
        }
        sock=SDLNet_TCP_Open(&server);
        if (!sock) {
            fprintf(stderr,"Cant connect to server\n");
            exit(3);
        }
        /* Spawn threads */
        SDL_Thread* threadNet = SDL_CreateThread(netThread, &d); //Creates network thread
        if(threadNet==NULL) {
            fprintf(stderr,"Error: threadNet\n");
            return -1;
        }
        SDL_Thread* threadUDP = SDL_CreateThread(udpThread, &d);
        if(threadUDP==NULL) {
            fprintf(stderr,"Error: threadUDP\n");
            return -1;
        }

        while(running) { //main threads handles event polling
            while (SDL_PollEvent(&EventData))
            {
                Event(&EventData);
            }
            SDL_Delay(1);
        }
        SDL_WaitThread(threadNet, NULL); //When the game exits, wait for the networking threads
        SDL_WaitThread(threadUDP, NULL);
    }
	SDL_WaitThread(threadRender, NULL); //When the program exits, wait for the render thread

	Cleanup(); //Before exit, clean up after the program
	return 0;
}
