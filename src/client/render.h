/*
 * File:   render.h
 * Author: Projektgrupp 4
 * Created by: Tobias Hultgren
 * Revisioned by:
 *
 * Created on April 10, 2011
 */

#define MAX_ZOOM 150
#define MIN_ZOOM 10

#define UNIT_STATUS_HEIGHT 32
#define UNIT_STATUS_WIDTH 64
#define HEALTHBARSIZE TILEDIAGONAL/20

//defines for menu textures
#define MENY 0
#define CONTINUE 1
#define HELP 2
#define OPTIONS 3
#define QUIT 4

//defines for tile textures
#define GRASS 1
#define WATER 2

//Initierar OpenGL för rendrering
void RenderInit(); //Tobias

//Updates the seen flag on all tiles
void UpdateSeenTiles(); //Tobias

//Loads all textures for the game
void LoadTextures(); //Tobias & Adam

//Cleans up textures when render exits
void RenderCleanup(); //Tobias

//Renders map tiles
void DrawMap(); //Tobias

//Renders decorations
void DrawDecorations(); //Tobias

//Renders menu
void DrawMenu(); //Adam

//Draws a line from one point to another
void DrawLine(float *from, float *to); //Tobiias

//Render startup menu
void DrawStartMenu(); //Adam

//Rendrerar enheter
void DrawUnits(); //Tobias

//Draws detailed game information in DEBUG mode
void DrawDetails(); //Tobias

//Draws list of player IDs and names
void DrawPlayerlist(); //Tobias

//Draws countdown timer
void DrawCountdown(); //Tobias

//Draws endgame status screen
void DrawEndgame(); //Tobias

/* Draws chat text */
void DrawChat(); //Christoffer

//Draws game HUD
void DrawHUD(); //Tobias

//Loads an image file to texture
GLuint *ImageToTexture(const char* file); //Adam & Tobias

//Changes the perspective (zoom/scroll) based on user interaction
void PerspectiveChange(); //Tobias

//Draws a textured rectangle
void DrawRectangle(int offset_setting,GLuint *textur,float x,float y,int width,int height); //Tobias

//Ritar ut text på skrämen
void DrawTextGame(int style, int x, int y, char *text, float *color); //Christoffer & Tobias
