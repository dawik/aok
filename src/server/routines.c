/*
 * File:   routines.c
 * Author: Projektgrupp 4
 * Created by: Tobias Hultgren
 * Revisioned by: David Wikmans
 *
 * Created on April 2, 2011
 */

#include "init.h"
#include "header.h"
#include "net.h"

void enough_players (struct client_t *clients, int *pool) {
	int x;
	char buffer[PACKETSIZE], temp[64];
	sprintf(temp,"waiting for players: ");
	for (x = 0; x < maxclients; x++) {
		if (pool[x] >= 0 && PLAYER_READY[x] < 0) {
			strcat(temp,clients[x].name);
			strcat(temp," ");
		}
	}
	sprintf(buffer,"%i#0#0#%s#%stype /ready!#",CHAT,"SERVER",temp);
	broadcast(pool,clients,buffer);
}

void send_win(struct client_t *clients, int id) {
	char buffer[PACKETSIZE];
	sprintf(buffer,"%i#0#0#0#0#0#",VICTORY);
	SDLNet_TCP_Send(clients[id].sock,buffer,strlen(buffer)+1);
	if (DEBUG) fprintf(stderr,"Sent %s to client %i\n",buffer,id);
}

void send_lose(struct client_t *clients, int id) {
	char buffer[PACKETSIZE];
	sprintf(buffer,"%i#0#0#0#0#0#",FAIL);
	SDLNet_TCP_Send(clients[id].sock,buffer,strlen(buffer)+1);
	if (DEBUG) fprintf(stderr,"Sent %s to client %i\n",buffer,id);
}


void set_resources(int amount) {
	int x;
	for(x=0; x<maxclients; x++) resources[x] = amount;
}

int tally_units(int id) {
	int total_units, x;
	for (total_units = 0, x = 0; x <= totalUnits; x++) {
		if (unit[x].class != 0)
			if (unit[x].owner == id)
				total_units++;
	}
	return total_units;
}

void shuffle_positions()
{
	int n = maxclients, i,j,t;
	if (n > 1) {
		for (i = 0; i < n - 1; i++) {
			j = i + rand() / (RAND_MAX / (n - i) + 1);
			t = playerStart[j].x;
			playerStart[j].x = playerStart[i].x;
			playerStart[i].x = t;
			t = playerStart[j].y;
			playerStart[j].y = playerStart[i].y;
			playerStart[i].y = t;
		}
	}
}

int decide_winner(int *pool, struct client_t *clients) {
	int x, players, winner = 0;
	SDL_Delay(1000);
	for (players = 0, x=0;x<maxclients;x++) {
		if (pool[x] != -1)
		{
			if (tally_units(x)>0 && PLAYER_DEAD[x] < 0) {
				players++;
				winner=x;
			} else{
				if(PLAYER_DEAD[x] != 1)
				{
					send_lose(clients,x);
					PLAYER_DEAD[x] = 1;
					resources[x] = 0;
				}
			}
			if (DEBUG) fprintf(stdout,"Player %i, units: %i total players: %i \n",x,tally_units(x), players);
		}
	}
	if (players<=1)
		return winner;
	else
		return -1;
}

void start_game(void *data) {
	struct shared_data* d = (struct shared_data*) data;
	char buffer[PACKETSIZE];
	int y, position[2], unit_id, count;
	ResetWorld();
	for (y = 0; y < maxclients; y++) {
		if (d->client_pool[y] >= 0) {
			sprintf(buffer,"%i#%i#%i#",START_COUNTDOWN,playerStart[y].x,playerStart[y].y);
			SDLNet_TCP_Send(d->clients[y].sock,buffer,strlen(buffer));
		}
	}
	count = SDL_GetTicks();
	GAME = COUNTDOWN;
	while (SDL_GetTicks() < count + 5000);
	GAME = GAME_ON;
	SDL_Delay(50);
	for (y = 0; y < maxclients; y++) {
		if (d->client_pool[y] >= 0) {
			position[0]=playerStart[y].x;
			position[1]=playerStart[y].y;
			unit_id=SpawnUnit(position,1,y);
			sprintf(buffer,"%i#%i#%i#%i#%i#%i",SPAWN_UNIT,playerStart[y].x,playerStart[y].y,unit_id, y ,1);
			broadcast(d->client_pool,d->clients,buffer);
			//SDLNet_TCP_Send(d->clients[y].sock,buffer,strlen(buffer));
			SDL_Delay(150);
		}
	}
}

void update_units(void *data) {
	struct shared_data* d = (struct shared_data*) data;
	char buffer[PACKETSIZE];
	int y;
	SDL_Delay(3000);
	for (y = 0; y < totalUnits; y++) {
		if (unit[y].class != 0) {
			sprintf(buffer,"%i#%i#%i#%i#%i#%i",SPAWN_UNIT,unit[y].position[0],unit[y].position[1], y, unit[y].owner, unit[y].class);
			broadcast(d->client_pool,d->clients,buffer);
		}
	}
}

int ready_check() {
	int ready_count,x;
	for (ready_count = 0, x = 0; x < maxclients; x++) {
		if (PLAYER_READY[x] >= 0)
			ready_count++;
	}
	if ((ready_count == number_of_clients) && number_of_clients > 1) {
		return 1;
	} else {
		return 0;
	}
}

void reset_ready() {
	memset(PLAYER_READY,-1,sizeof(int)*maxclients);
}

void reset_dead() {
	memset(PLAYER_DEAD,-1,sizeof(int)*maxclients);
}


int SpawnUnit(int* position, int class, int owner)
{
	int i = 0;
	if (GAME != COUNTDOWN)
		for(i=0; i<MAX_UNITS; i++)
			if(unit[i].class == 0){
				unit[i].target = -1;
				unit[i].target_position[0] = -1;
				unit[i].target_position[1] = -1;
				unit[i].class = class;
				unit[i].position[0] = position[0];
				unit[i].position[1] = position[1];
				unit[i].target_position[0] = -1;
				unit[i].target_position[1] = -1;
				unit[i].destination[0] = -1;
				unit[i].destination[1] = -1;
				unit[i].selected = false;
				unit[i].waitto = 0;
				unit[i].status = IDLE;
				unit[i].id = i;
				unit[i].facing = SOUTH;
				unit[i].owner = owner;
				unit[i].nextattack = 0;
				unit[i].dead = false;
				unit[i].width = unit_class[class].width;
				unit[i].height = unit_class[class].height;
				unit[i].speed = unit_class[class].speed;
				unit[i].health = unit_class[class].health;
				unit[i].damage = unit_class[class].damage;
				unit[i].range = unit_class[class].range;
				unit[i].attackspeed = unit_class[class].attackspeed;
				unit[i].canwork = unit_class[class].canwork;
				unit[i].sight = unit_class[class].sight;
				unit[i].path_ticks = PATHTICKS;
				map[position[0]][position[1]].unit = &unit[i];
				if(DEBUG) fprintf(stderr,"Spawning unit: %d at %d,%d owner: %i\n",unit[i].id, unit[i].position[0], unit[i].position[1], unit[i].owner);
				if(totalUnits < i) totalUnits = i;
				if (DEBUG) fprintf(stderr,"Spawning unit %i class %i speed %i health %i damage %i sight %i\n",i,unit[i].class,unit[i].speed,unit[i].health,unit[i].damage,unit[i].sight);
				if (GAME != WARMUP)
					if (resources[owner] - unit_class[class].price >= 0) resources[owner] -= unit_class[class].price;
				break;
			}
	return i;
}

int SetDestination(int* selectedTile, int id)
{
	if(map[selectedTile[0]][selectedTile[1]].walkable == 0   //Kontrollera om klicket var på en icke-walkable tile eller utanför banan, och ange då ingen destination
			|| selectedTile[0] < 0 || selectedTile[0] >= MAP_WIDTH
			|| selectedTile[1] < 0 || selectedTile[1] >= MAP_HEIGHT
	  ) return false;

	SDL_LockMutex(unitlock[id]);
	unit[id].destination[0] = selectedTile[0];
	unit[id].destination[1] = selectedTile[1];
	unit[id].status = GETPOS;
	SDL_UnlockMutex(unitlock[id]);

	return true;
}

void KillUnit(void *data, int unit_id)
{
	if(DEBUG) fprintf(stderr,"Killing unit %i!\n",unit_id);
	struct shared_data* d = (struct shared_data*) data;
	char buffer[100];
	map[unit[unit_id].position[0]][unit[unit_id].position[1]].unit = NULL;
	unit[unit_id].class = 0;
	sprintf(buffer,"100#%i#0#0#",unit_id);
	broadcast(d->client_pool, d->clients, buffer);
}
