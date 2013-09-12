/*
 * File:   header.h
 * Author: Projektgrupp 4
 * Created by: Tobias Hultgren
 * Revisioned by:
 * Emil L, 110502
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
#define MAX_BUILDINGS 200
#define MAX_UNIT_CLASSES 100
#define MAX_CONNECTIONS 8
#define PATHLENGTH 300

#define BOX_LIMIT 10

float SCROLLSPEED; //defines the speed of game scrolling
int ZOOMSPEED; //defines how fast the scroll zooms
int TILESIZE;
int WINDOWHEIGHT;
int WINDOWWIDTH;
int MAP_WIDTH;
int MAP_HEIGHT;
int FRAMERATE;
int PACKETSIZE;
int MESSAGESIZE;
int UNIT_CLASSES;
int HUD_HEIGHT;
int HUD_LINE_HEIGHT;
int HUD_ICON_PADDING;
int MAX_POPULATION;
float TILEDIAGONAL;

SDL_mutex *unitlock[MAX_UNITS];

enum pathing_status { IDLE, MOVING, FOLLOWING, ATTACKING, WORKING };
enum directions { NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NORTHWEST };
enum sounds { ATTACK, MUSIC, MOVE, CANNOT, MENU, DECREASE, INCREASE, FOLLOW, LOST, WON, UNDER_ATTACK, WORK, UNIT_LOST };

struct Point {
	int x;
	int y;
};

#define UNIT_DIRECTIONS 8
//Defines maximum possible textures of each kind
#define MAX_TEXTURES_TILE 15
#define MAX_TEXTURES_MENU 30
#define MAX_TEXTURES_DECORATION 101
#define MAX_TEXTURES_STATUS 5
#define MAX_TEXTURES_HUD UNIT_CLASSES
#define MAX_TEXTURES_COUNTDOWN 6
#define MAX_TEXTURES_ENDGAME 2
#define MAX_TEXTURES_MMHUD 19
struct texture_types //Tobias
{
    GLuint *tile[MAX_TEXTURES_TILE];
    GLuint *menu[MAX_TEXTURES_MENU];
    GLuint *decoration[MAX_TEXTURES_DECORATION];
    GLuint *unit[MAX_UNIT_CLASSES][UNIT_DIRECTIONS];
    GLuint *status[MAX_TEXTURES_STATUS];
    GLuint *hud[30];
    GLuint *countdown[MAX_TEXTURES_COUNTDOWN];
    GLuint *endgame[MAX_TEXTURES_ENDGAME];
    GLuint *mmhud[MAX_TEXTURES_MMHUD];
} texture;

struct settings_struct //Tobias
{
    char username[50];
    char server[100];
    int port;
    char mapname[50];
    int textures;
    int interpolation;
    int fullscreen;
    float boxcolor[3];
    int boxborder;
    float playercolor[MAX_CONNECTIONS][3];
    int volume;
    float shader;
    int consolerows;
} settings;

struct Unit_struct //Tobias
{
    struct Point path[PATHLENGTH];
    int pathLength;
    enum pathing_status status;
    int destination[2];
    int position[2];
    int target;
    int group;

    float fromcoord[2]; //Coordinates from last tile
    float coord[2]; //Current coordinates
    float destcoord[2]; //Coordinates for next tile
    enum directions facing; //Unit facing
    int selected; //Defines if the unit is selected
    int id; //Unit ID, same as position in unit structure array
    int waitto;
    int fromtime; //Time data from last tile

    int canwork;
    int owner; //Client ID of the unit owner
    int class;
    float width; //Multiple of unit size relative to tile size
    float height;

    int speed; //How long units need to move from one tile to another
    int health; //Current unit health
    int maxhealth; //Maximum unit health
    int damage; //Unit damage
    int range; //Unit range
    int attackspeed; //Unit attack speed
    int nextattack;

    int sight;
} unit[MAX_UNITS];

struct Unit_class_struct //Tobias
{
    int class;
    float width;
    float height;
    int speed; //unit wait time in ms
    int health;
    int damage;
    int range;
    int attackspeed;
    int sight; //unit sight range in tiles
    int canwork;
    int price;
} unit_class[MAX_UNIT_CLASSES];

struct Tile //Tobias
{
    int background;
    int walkable;
    int decoration;
    struct Unit_struct* unit;
    struct Building_struct* building;
    int seen;
} map[MAP_WIDTH_MAX][MAP_HEIGHT_MAX];

/* shared among threads */
struct shared_data {
    char name[32];
    SDL_Event event;
};

TCPsocket sock;
UDPsocket usock;

int randomsound;
int musicVolume;
int gameVolume;
int totalUnits;
int maxPlayers;
int running;
int connected;
int hudClick;
int menuOpen;
int mapmakerOpen;
int startMenuOpen;
int resources;
int population;
int countdown;
int endgameStatus;
int showEndgameStatus;
struct SDL_Surface* Screen;

float offset[2];
int offsetflag[2];
int offsetflagmouse[2];
int offsetflagscroll[2];
int scrollDownStart[2];
int shiftDown;
int altDown;
int ctrlDown;
int tabDown;
int scrollDown;
int leftMouseDown;
int capsActive;
int zoomSchedueled;
int lastSelectedGroup;
int lastAttacked;
struct Point playerStart[MAX_CONNECTIONS];

int boxStart[2];
int boxEnd[2];
int boxDiff[2];

int client_id;

#define CHAT_MESSAGES_BUFFER 50
int drawconsole;
char console[200];
char messagebuffer[CHAT_MESSAGES_BUFFER][200];

char connectedClient[MAX_CONNECTIONS][50];

//Initierar nödvändiga variabler till sina startvärden och startar SDL
int Init(); //Tobias

//Event-funktionen hanterar events som knapptryckningar och musklick
void Event(SDL_Event* event); //Tobias (in-game) & Christoffer (chat)

//Huvudfunktion för render-tråden
int Render(); //Tobias

//Initiates the render module
void RenderInit(); //Tobias

//Draws the map
void DrawMap(); //Tobias

//Draws all units
void DrawUnits(); //Tobias

//Körs då programmet avslutas för att rensa upp bland använda resurser
void Cleanup(); //Tobias

//Starts up the map maker
void MapMaker(); //Fredrik

//SetSelected tar in aktuell musposition och bestämmer vilka enheter som ska vara markerade
int SetSelected(int* mPos); //Tobias

//SetTarget test the selected units target to the unit pointed at. Returns target ID if successful, otherwise -1
int SetTarget(int* mPos); //Tobias

//SendUnits komponerar en sträng units som är markerade samt den tile/enhet man klickar på och skickar till server.
int SendUnits(int* position, int target_unit); //David

//Determines clicked HUD button and performes respecive action. Returns button ID.
int ClickHUDButton(int *mPos); //Tobias

//SpawnUnit skapar en enhet av typen class på angiven position
int SpawnUnit(int* position, int class, int unit_id, int owner); //Tobias

//KillUnit raderar en given enhet 
void KillUnit(int unit_id); //David

//Parsar och förflyttar enheter
void MoveUnits(char* string); //David & Tobias

//Tar in värden för musens position och gör om dessa värden till den isometriska positionen på kartan med hänsyn till offset
void GetIsometricPoint(int* position); //Tobias

//Sets a group marking to selected
void SetSelectedGroup(); //Tobias

//Sets the selected units to the specified control group
void SetControlGroup(int id); //Tobias

//Sets units from the specified control group to selected
void GetControlGroup(int id); //Tobias

//Sound functions
void PlayGameSound(enum sounds id); //Emil
void ChangeGameVolume(enum sounds id); //Emil
void ChangeMusicVolume(enum sounds id); //Emil
void ChangeMusic(); //Emil

#endif
