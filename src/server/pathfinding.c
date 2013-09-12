/*
 * File:   pathfinding.c
 * Author: Projektgrupp 4
 * Created by: Christian Samuelsson
 * Revisioned by: David Wikmans
 *
 * Created on April 7, 2011
 */

#include "header.h"
#include "pathfinding.h"
#include "net.h"

/* Finds the point the is connected to G with the smallest value */
/* Only scans "infolist", the OPEN list */
struct Point Minimum(int info[MAP_WIDTH][MAP_HEIGHT] ,int G[MAP_WIDTH][MAP_HEIGHT]){
	struct Point ret = {-1,-1};
	int i,min=100000;
	for(i=0; i<SCAN_LIMIT; i++){
        int x,y;
        x = infolist[i].x;
        y = infolist[i].y;
        if(info[x][y] == OPEN){
            if(G[x][y] < min) {
                min = G[x][y];
                ret.x = x;
                ret.y = y;
            }
        }
	}
	return ret;
}

/* Finds a point that is close to "source" and empty from units and trees */
/* This is so that you can send many units to the same point */
struct Point FindClosestEmpty(struct Point src){
    int addlen=3, s=0,x,y,n;
    struct Point p;
    for(n=0; n<50; n++){
        s++;
        for(x=0; x<addlen; x++){
            for(y=0; y<addlen; y++){
                if(src.x == (src.x-s+x) && src.y == (src.y-s+y)) continue;
                if(map[src.x-s+x][src.y-s+y].walkable){
                    if(map[src.x-s+x][src.y-s+y].unit == NULL){
                        p.x = src.x-s+x;
                        p.y = src.y-s+y;
                        return p;
                    }
                }
            }
        }
        addlen+=2;
    }
    p.x = src.x;
    p.y = src.y;
    return p;
}

/* Returns (fills "closest") with 8 points that is neighbour to "source" */
void GetClosest(struct Point *closest, struct Point source, int info[MAP_WIDTH][MAP_HEIGHT]){
	int xlist[8] = {-1,-1,0,1,1,1,0,-1};
	int ylist[8] = {0,-1,-1,-1,0,1,1,1};
	int n;
	struct Point none = {-1,-1};
	for(n=0;n<8;n++){
		int cx = source.x + xlist[n];
		int cy = source.y + ylist[n];
		/* x/ylist instead of alot of IF's */
		/* Checks if point is within map borders */
		if(cx >= 0 && cy >= 0 && cx < MAP_WIDTH && cy < MAP_HEIGHT){
			if(map[cx][cy].walkable == 1){
			    struct Point p = {cx, cy};
			    if(map[cx][cy].unit == NULL){
                    closest[n] = p;
			    } else if(map[cx][cy].unit->status != IDLE && map[cx][cy].unit->status != WAITING){
			        closest[n] = p;
			    } else closest[n] = none;
			} else closest[n] = none;
		} else closest[n] = none;
	}
	/* Sets so that a neighbour is NOT behind a diagonal wall */
	if(closest[0].x == -1 && closest[2].x == -1) closest[1]=none;
	if(closest[2].x == -1 && closest[4].x == -1) closest[3]=none;
	if(closest[4].x == -1 && closest[6].x == -1) closest[5]=none;
	if(closest[6].x == -1 && closest[0].x == -1) closest[7]=none;
}

/* A function that speeds up the sorting of OPEN points instead of searching the whole map */
void SetInfo(int info[MAP_WIDTH][MAP_HEIGHT], struct Point p, int set){
    int i,found=0, empty=-1;
    info[p.x][p.y] = set;
    for(i=0; i<SCAN_LIMIT; i++){
        if(infolist[i].x == -1 && empty==-1) {
            empty=i;
        }
        if(infolist[i].x == p.x && infolist[i].y == p.y){
            found=1;
            if(set == CLOSED){
                infolist[i].x = -1;
                infolist[i].y = -1;
            } else if(set == OPEN){
                infolist[i].x = p.x;
                infolist[i].y = p.y;
            }
            return;
        }
    }
    if(infolist[empty].x == -1){ /* This is true if there is no allocated memory left */
        infolist[empty].x = p.x;
        infolist[empty].y = p.y;
    } else fprintf(stderr, "Path: NO EMPTY SLOT FOUND! DETTA ÄR INTE BRA!\n");
}

/* The main pathfinding algorithm */
/* The startpoint is "start" and endpoint is "end". It fills the "path" pointer with the resulting path and length with pathlength */
int Dijkstras(struct Point start, struct Point end, struct Point *path, int *length) {
	int i=0,n,found=-1;

	struct Point parent	[MAP_WIDTH][MAP_HEIGHT];
	int info	        [MAP_WIDTH][MAP_HEIGHT];
	int G 		        [MAP_WIDTH][MAP_HEIGHT];

	struct Point check;
	struct Point none = {-1,-1};
	struct Point p = {-1,-1};

	for(i=0;i<MAP_HEIGHT; i++){ /* Sets all temp arrays with default values */
		for(n=0; n<MAP_WIDTH; n++){
			info[i][n]      = 0;
			parent[i][n]    = none;
			G[i][n]         = 0;
		}
	}

    for(i=0; i<SCAN_LIMIT; i++){
        infolist[i].x = -1;
        infolist[i].y = -1;
    }
	
    SetInfo(info, start, OPEN);
	while(found == -1){ /* Tries to find the path until it is found or no more tiles to scan */

		struct Point min;
		min = Minimum(info, G);

		if(min.x == -1) {found=0; break;}
		check = min;

		int checkG = G[check.x][check.y];
		SetInfo(info, check, CLOSED);

		struct Point closest[8];
		GetClosest(closest, check, info);

		for(n=0;n<8;n++){
			int cx = closest[n].x;
			int cy = closest[n].y;
			if(cx != -1 && cy != -1){
				int len = ((n==1 || n==3 || n==5 || n==7) ? 14 : 10) + checkG; 
				/* It wants to go in a stright line MORE than diagonally */ 
				/* 14 is the length to any diagonal tile, 10 is the length to a tile in a stright line*/
				if(info[cx][cy] == OPEN){
					if(len < G[cx][cy]){
						G[cx][cy] = len;
						parent[cx][cy] = check;
					}
				} else if(info[cx][cy] == 0){
					parent[cx][cy] 	= check;
					G[cx][cy] = len;
				    p.x = cx;
				    p.y = cy;
				    SetInfo(info, p, OPEN);
				}
				if(cx == end.x && cy == end.y) { /* If it was found! */
					found = 1;
					break;
				}
			}
		}
		if(found != -1) break;
	}

	if(!found || found == -1)
		return 0;
	else {
		int len = 0;
		struct Point p = end;
		/* Now fills the path pointer with the resulting path */
		/* Goes through the acesstor-tree from the endpoint all the way to the beginning */
		for(i=0; i<PATHLENGTH; i++){
			if(p.x == start.x && p.y == start.y) break;
			path[len++] = p;
			p = parent[p.x][p.y];
		}

		*length = len-1;
		return 1;
	}
}

/* The thread that calculates paths */
int pathThread (void *data) {
	int i, dx, dy, dij, dist;
	struct Point me, tar, next;
	while (running) {
		for (i = 0; i <= totalUnits ; i++) {
		    if(unit[i].class != 0){
                if(unit[i].target != -1){
                    if(unit[unit[i].target].class != 0){
                        SDL_LockMutex(unitlock[i]);

                        me.x = unit[i].position[0];
                        me.y = unit[i].position[1];
                        tar.x = unit[unit[i].target].position[0];
                        tar.y = unit[unit[i].target].position[1];

                        dx = abs(me.x-unit[unit[i].target].position[0]);
                        dy = abs(me.y-unit[unit[i].target].position[1]);
                        dist = sqrt(dx*dx + dy*dy);
			/* Checks if the distance of the path is to be too long */

                        if((unit[i].position[0] != unit[i].target_position[0] //Checks if the target unit has moved or has no target position
                           && unit[i].position[1] != unit[i].target_position[1]) || dist >= 3)
                        {
                            next = FindClosestEmpty(tar);
                            if((dij = Dijkstras(me, next, unit[i].path, &unit[i].pathLength))) {
                                unit[i].status = MOVING;
                                unit[i].target_position[0] = unit[i].destination[0] = next.x;
                                unit[i].target_position[1] = unit[i].destination[1] = next.y;
                            }
                            else {
                                unit[i].status = IDLE;
                                unit[i].pathLength = 0;
                            }
                        }
                        SDL_UnlockMutex(unitlock[i]);
                    } else { /* Target är DÖD! */
                        SDL_LockMutex(unitlock[i]);
                        unit[i].target = -1;
                        SDL_UnlockMutex(unitlock[i]);
                    }
                } else if(unit[i].status == GETPOS){
                    struct Point start, end;
                    start.x = unit[i].position[0];
                    start.y = unit[i].position[1];
                    end.x = unit[i].destination[0];
                    end.y = unit[i].destination[1];
                    dx = abs(end.x-start.x);
                    dy = abs(end.y-start.y);
                    dist = sqrt(dx*dx + dy*dy);

                    if(dist <= MAX_CLICKLENGTH){
                        SDL_LockMutex(unitlock[i]);
                        struct Point tmpdest;
                        if(map[end.x][end.y].unit == NULL){
                            tmpdest = end;
                        } else {
                            tmpdest = FindClosestEmpty(end);
                            unit[i].destination[0] = tmpdest.x;
                            unit[i].destination[1] = tmpdest.y;
                        }

                        if (DEBUG) fprintf(stderr, "Unit %d: Pathfinding!\n", i);
                        if(tmpdest.x != -1 && (dij = Dijkstras(start, tmpdest, unit[i].path, &unit[i].pathLength)))
                        {
                            unit[i].status = MOVING;
                        }
                        else
                        {
                            unit[i].status = IDLE;
                            unit[i].pathLength = 0;
                        }
                        SDL_UnlockMutex(unitlock[i]);
                    }
                }
            }
		}
		SDL_Delay(10);
	}
	return 0;
}

