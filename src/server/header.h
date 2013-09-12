/*
 * File:   header.h
 * Author: Projektgrupp 4
 * Created by: Tobias Hultgren
 * Revisioned by: Tobias Hultgren
 *
 * Created on April 2, 2011
 */

#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include "includes.h"

#define true (0==0)
#define false (!true)

#define DEBUG false
#define MAP_WIDTH_MAX 1000
#define MAP_HEIGHT_MAX 1000
#define MAX_UNITS 5000
#define MAX_UNIT_CLASSES 100
#define PATHLENGTH 300
#define PATHTICKS 20
#define minpacket 6
#define maxclients 8


int PACKETSIZE;
int UNIT_CLASSES;

int MAP_WIDTH;
int MAP_HEIGHT;

int number_of_clients;
int running;
int totalUnits;
int total_spawnpoints;

int MAX_CLICKLENGTH;

SDL_mutex *unitlock[MAX_UNITS];
SDL_mutex *netlock[maxclients];
enum pathing_status { IDLE, MOVING, GETPOS, WAITING  };
enum directions { NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NORTHWEST };

struct Point {
	int x;
	int y;
};

char mapname[32];

struct Unit_struct //Tobias
{
    struct Point path[PATHLENGTH];
    int pathLength;
    enum pathing_status status;
    int destination[2];
    int position[2];
    int sight;
    int target;
    int target_position[2];
    int path_ticks;

    float coord[2];
    float destcoord[2];
    enum directions facing;
    int selected;
    int id;
    int waitto;
    int dead;

    int owner;
    int class;
    float width;
    float height;

    int speed;
    int health;
    int damage;
    int range;
    int attackspeed;
    int nextattack;
    int canwork;
};

struct Unit_class_struct //Tobias
{
    int class;
    float width;
    float height;
    int speed;
    int health;
    int damage;
    int range;
    int attackspeed;
    int sight;
    int canwork;
    int price;
};

struct Tile //Tobias
{
    int background;
    int walkable;
    int decoration;
    int seen[maxclients];
    struct Unit_struct* unit;
};

struct Tile map[MAP_WIDTH_MAX][MAP_HEIGHT_MAX];
struct Unit_struct unit[MAX_UNITS];
struct Unit_class_struct unit_class[MAX_UNIT_CLASSES];
struct Point playerStart[maxclients];

char servername[100];
int serverport;
int resources[maxclients];

IPaddress ip;
TCPsocket sock;
UDPsocket usock;

//Initierar nödvändiga variabler till sina startvärden och startar SDL
int Init(); //David & Tobias

//Utför beräkningar som skall göras varje loop
int Loop(); //David, Christoffer & Tobias

//Updates the seen tiles for all clients
void UpdateSeenTiles(); //Tobias

//Handles sessions
int sessionThread(void *data); //David

/*PathingThread */
int pathThread(void *data); //Christoffer & David

//SpawnUnit skapar en enhet av typen class på angiven position
int SpawnUnit(int* position, int class, int owner); //Tobias

//Sets destination to unit
int SetDestination(int* selectedTile, int id); //Tobias & David

//KillUnit raderar en enhet från spelet från angiven position
void KillUnit(void *data, int unit_id); //Tobias & David

int Dijkstras(struct Point start, struct Point end, struct Point *path, int *len); //Christoffer

void set_resources(int amount); //David
int ready_check(); //David
void reset_ready(); //David
void start_game(void *data); //David
void reset_dead(); //David

#endif
