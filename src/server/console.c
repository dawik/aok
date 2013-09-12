/*
File: console.c
By: David Wikmans
Created 13 april, 2011.
 */

#include "init.h"
#include "header.h"
#include "net.h"

/* Thread to manage server -- David */

int consoleThread(void *data) {
	struct shared_data* d = (struct shared_data*) data;
	char cmd[3][256], buffer[PACKETSIZE], *ptr;
	int x;
	while (running) {

		printf("enter commmand\n");
		fgets(cmd[0], 256, stdin);
		cmd[0][strlen(cmd[0])-1]='\0';
		ptr = strtok(cmd[0]," ");
		if (ptr != NULL)
			strcpy(cmd[1],ptr);
		ptr = strtok(NULL,"#");
		if (ptr != NULL)
			strcpy(cmd[2],ptr);
		/* Reset game */
		if (!strcmp(cmd[1],"/reset")) {
			ResetWorld();
			snprintf(buffer,PACKETSIZE,"9999#0#0#0#0#");
			broadcast(d->client_pool,d->clients,buffer);
			GAME = WARMUP;
			reset_ready();
			reset_dead();
			fprintf(stdout,"World reset\n");
			/* Quit server */
		} else if (!strcmp(cmd[1],"/quit")) {
			snprintf(buffer,PACKETSIZE,"1#0#0#CONSOLE#Server shutting down#");
			broadcast(d->client_pool,d->clients,buffer);
			fprintf(stdout,"Server going dooooown\n");
			exit(1);
			/* Go to warmup mode */
		} else if (!strcmp(cmd[1],"/warmup")) {
			snprintf(buffer,PACKETSIZE,"1#0#0#CONSOLE#Going to warmup mode#");
			for (x=0; x < maxclients; x++)
				PLAYER_READY[x] = -1; 
			GAME = 0;
			ResetWorld();
			broadcast(d->client_pool,d->clients,buffer);
			/* List connected clients -- ready == 1 not ready == -1 */
		} else if (!strcmp(cmd[1],"/who")) {
			printClients(d->client_pool,d->clients);
			/* List which client has udp enabled */
		} else if (!strcmp(cmd[1],"/udp")) {
			for (x=0; x < maxclients; x++) {
				fprintf(stdout,"client %i udp %i\n",x, d->clients[x].udp);
			}
			/* Remove client from server */
		} else if (!strcmp(cmd[1],"/kick")) {
			dropclient(d,atoi(cmd[2]));
			/* Load map from folder maps/ */
		} else if (!strcmp(cmd[1],"/load")) {
			sprintf(buffer,"%s.dat",cmd[2]);
			if (LoadMap(buffer) == 0) {
				strcpy(mapname,cmd[2]);
				for (x = 0; x < maxclients; x++) {
					if (d->client_pool[x] >= 0)
						cmdsend(HANDSHAKE,x,resources[x],mapname,NULL,d->clients[x].sock);
				}
			}
			/* In game chat as CONSOLE */
		} else if (!strcmp(cmd[1],"/say")){
			snprintf(buffer,PACKETSIZE,"1#0#0#%s#%s","CONSOLE",cmd[2]);
			broadcast(d->client_pool,d->clients,buffer);
		}   
}
return 0;
}

