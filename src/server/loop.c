/*
 * Author: Projektgrupp 4
 * Created by: Tobias Hultgren
 * Revisioned by: Christoffer Samuelsson
 * Re-revisioned by: David Wikmans -- May 02, 2011
 * Created on April 2, 2011
 */

#include "header.h"
#include "net.h"

int CanMove(struct Point p1, struct Point p2){
	int dx, dy;
	dx = p2.x - p1.x;
	dy = p2.y - p1.y;
	if(p1.x+dx >= 0 && p1.x+dx < MAP_WIDTH && p1.y+dy >= 0 && p1.y+dy < MAP_HEIGHT)
	{
	    if(map[p1.x+dx][p1.y+dy].unit == NULL && map[p1.x+dx][p1.y+dy].walkable){
			if(map[p1.x+dx][p1.y].unit != NULL && map[p1.x][p1.y+dy].unit != NULL){
				if(map[p1.x+dx][p1.y].walkable && map[p1.x][p1.y+dy].walkable){
					return 0;
				}
			}
		} else {
			return 0;
		}
	}
	return 1;
}

void UpdateSeenTiles()
{
	int x, y, i;
	for(x=0; x<MAP_WIDTH; x++)
		for(y=0; y<MAP_HEIGHT; y++)
			for(i=0; i<maxclients; i++)
				map[x][y].seen[i] = 0; //Makes all tiles unseen

	for(i=0; i<=totalUnits; i++) //Makes tiles within sight range seen
		if(unit[i].class != 0)
			for(x = unit[i].position[0]-unit[i].sight; x <= unit[i].position[0]+unit[i].sight; x++)
				for(y = unit[i].position[1]-unit[i].sight; y <= unit[i].position[1]+unit[i].sight; y++)
				{
					if(x < 0 || x >= MAP_WIDTH || y < 0 || x >= MAP_HEIGHT) continue; //Don't edit tiles that do not exist as it would cause segfaults
					map[x][y].seen[unit[i].owner] = 1;
				}
}

int Loop(void *data)
{
	int startTimer, unitsMoving, healthChanges, resourceChange, pingCrement = 0, nagCrement = 0;
	char temp[PACKETSIZE], moveStr[PACKETSIZE], healthStr[PACKETSIZE], buffer[PACKETSIZE];
	struct shared_data* d = (struct shared_data*) data;

	if(DEBUG) fprintf(stderr, "Loop thread started!\n");

	while(running)
	{
		UpdateSeenTiles();
		int i;
		for(i=0, temp[0] = '\0', buffer[0] = '\0', moveStr[0] = '\0', healthStr[0] = '\0', unitsMoving = 0, healthChanges = 0, resourceChange = 0; i<=totalUnits; i++)
		{
			if(unit[i].waitto > SDL_GetTicks()) continue;

			startTimer = SDL_GetTicks();
			unit[i].waitto = startTimer + unit[i].speed;

			if(unit[i].class != 0){
				if(unit[i].health <= 0) //Kill the unit if it has no health
				{
					SDL_LockMutex(unitlock[i]);
					KillUnit(d, i);
					SDL_UnlockMutex(unitlock[i]);
					continue;
				}

				if(unit[i].target != -1) //Attack the target unit if it has reached its cooldown and the target is in range
				{
					if(unit[unit[i].target].owner != unit[i].owner) //Check that the target is hostile
					{
						if(   abs(unit[unit[i].target].position[0] - unit[i].position[0]) <= unit[i].range
								&& abs(unit[unit[i].target].position[1] - unit[i].position[1]) <= unit[i].range) //Check that the target is in range
							if(unit[i].nextattack < SDL_GetTicks()) //Check that the unit has reached its attack cooldown
							{

								unit[unit[i].target].health -= unit[i].damage;
								if (DEBUG) fprintf(stderr,"Unit %i attacked %i. Health: %i\n",i,unit[i].target,unit[unit[i].target].health);
								unit[i].nextattack = SDL_GetTicks() + unit[i].attackspeed;
								/* compose health string */
								sprintf(temp,"%i#%i#",unit[i].target, unit[unit[i].target].health);
								if (strlen(healthStr)<PACKETSIZE) {
									strcat(healthStr,temp);
									healthChanges++;
								}
							}
						if( map[unit[unit[i].target].position[0]][unit[unit[i].target].position[1]].seen[unit[i].owner] == 0 ) //Checks if the target enemy is on a seen tile
						{
							unit[i].target = -1;
							unit[i].status = IDLE;
						}
					}
				}
				else //If the unit doesn't have a target, try to find one within sight
				{
					int x, y, i;
					for(i=0; i<=totalUnits; i++)
						if(unit[i].class != 0)
						{
							if(unit[i].canwork == 0) //Only units with damage tries to find a target
							{
								for(x = unit[i].position[0]-unit[i].sight; x <= unit[i].position[0]+unit[i].sight; x++)
									for(y = unit[i].position[1]-unit[i].sight; y <= unit[i].position[1]+unit[i].sight; y++)
									{
										if(x < 0 || x >= MAP_WIDTH || y < 0 || x >= MAP_HEIGHT) continue; //Don't edit tiles that do not exist as it would cause segfaults
										if(map[x][y].unit != NULL)
											if(map[x][y].unit->owner != unit[i].owner)
												unit[i].target = map[x][y].unit->id;
									}
							}
							else //Workers mine for resources if possible
								if(unit[i].nextattack < SDL_GetTicks()) //Checks that the units can collect resources at this point
								{
									for(x = unit[i].position[0] - 1; x <= unit[i].position[0] + 1; x++)
										for(y = unit[i].position[1] - 1; y <= unit[i].position[1] + 1; y++)
										{
											if(x < 0 || x >= MAP_WIDTH || y < 0 || x >= MAP_HEIGHT) continue; //Don't examine non-existant tiles
											if(map[x][y].decoration == 4 && GAME != COUNTDOWN) //Checks if the current tile contains resources
											{
												resources[unit[i].owner]++;
												resourceChange = 1;
												if (DEBUG) fprintf(stderr,"Unit %i owner: %i collected resources\n",i,unit[i].owner);
												unit[i].nextattack = SDL_GetTicks() + unit[i].attackspeed;
												x = unit[i].position[0] + 2; //Used to break out of both loops
												y = unit[i].position[1] + 2;
											}
										}
								}
						}
				}

				struct Point start, end, next;
				start.x = unit[i].position[0];
				start.y = unit[i].position[1];
				end.x = unit[i].destination[0];
				end.y = unit[i].destination[1];

				if(unit[i].status == WAITING){
					SDL_LockMutex(unitlock[i]);
					unit[i].status = GETPOS;
					SDL_UnlockMutex(unitlock[i]);
					continue;
				}
				next.x = unit[i].path[unit[i].pathLength].x;
				next.y = unit[i].path[unit[i].pathLength].y;

				if(unit[i].status == MOVING){
					SDL_LockMutex(unitlock[i]);
					if(CanMove(start,next)){
						map[start.x][start.y].unit = NULL;
						unit[i].position[0] = next.x;
						unit[i].position[1] = next.y;
						map[unit[i].position[0]][unit[i].position[1]].unit = &unit[i];
						unit[i].pathLength --;

						/* compose movement info */
						sprintf(temp,"%i#%i#%i#",i, next.x, next.y);
						if (strlen(moveStr)<PACKETSIZE) {
							strcat(moveStr,temp);
							unitsMoving++;
						}
						//Sets unit timer
						unit[i].waitto = startTimer + unit[i].speed;
					} else {
						if(map[next.x][next.y].unit != NULL){ /* Enhet framför */
							if(map[next.x][next.y].unit->status == IDLE){ /* Not idle */
								unit[i].status = GETPOS;
								SDL_UnlockMutex(unitlock[i]);
								continue;
							} else {
								if(unit[i].path_ticks>0){
									unit[i].status=WAITING;
									if (DEBUG) fprintf(stderr, "Unit %d: Waiting!\n", i);
									unit[i].path_ticks--;
									SDL_UnlockMutex(unitlock[i]);
								} else {

									unit[i].status=IDLE;
									if (DEBUG) fprintf(stderr, "Unit %d: Stopping!\n", i);
									unit[i].path_ticks = PATHTICKS;
									SDL_UnlockMutex(unitlock[i]);
								}
								continue;
							}
						} else { /* Enhet ej framför */
							unit[i].status=IDLE;
							if (DEBUG) fprintf(stderr, "Unit %d: Stopping!\n", i);
							SDL_UnlockMutex(unitlock[i]);
							continue;
						}
					}
					SDL_UnlockMutex(unitlock[i]);
					/* if(DEBUG) fprintf(stderr,"Moving %d to %d,%d\n", unit[i].id, unit[i].position[0], unit[i].position[1]); */

					int sx = unit[i].position[0], sy=unit[i].position[1], dx=unit[i].destination[0], dy=unit[i].destination[1];
					if(sx == dx && sy == dy){
						SDL_LockMutex(unitlock[i]);
						unit[i].status = IDLE;
						unit[i].pathLength = 0;
						SDL_UnlockMutex(unitlock[i]);
						continue;
					}
				}
			} /* UNITS */
			/* Regulate packet size */
			if (strlen(moveStr) + strlen(healthStr) > PACKETSIZE-128)
				break;
		}
		pingCrement++;
		nagCrement++;
		if (unitsMoving>0) {
			sprintf(buffer,"M#%i#",unitsMoving);
			strcat(buffer,moveStr);
			if (DEBUG) fprintf(stdout,"moving units:%i str: %s\n", unitsMoving, buffer);
			//tcpcast(d->client_pool,d->clients,buffer);
			//ubroadcast(d->client_pool,d->clients,buffer);
		} else {
			sprintf(buffer,"M#-1#");
		}
		if (healthChanges>0) {
			sprintf(temp,"H#%i#",healthChanges);
			strcat(temp,healthStr);
			strcat(buffer,temp);
			//ubroadcast(d->client_pool,d->clients,buffer);
			//tcpcast(d->client_pool,d->clients,buffer);
		} else {
			sprintf(temp,"H#-1#");
			strcat(buffer,temp);
		}
		if (unitsMoving > 0 || healthChanges > 0 || resourceChange > 0 || pingCrement == 20) {
			update_stats(d->client_pool, d->clients,buffer);
			if (pingCrement == 20)
				pingCrement = 0;
		}
		if (nagCrement == 600 && number_of_clients > 1) {
			enough_players(d->clients, d->client_pool);
			nagCrement = 0;
		}
		SDL_Delay(100);
	}
	return 1;
}
