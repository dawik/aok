/*
 * File:   mapmaker.h
 * Author: Projektgrupp 4
 * Created by: Fredrik Berg
 * Revisioned by:
 *
 * Created on May 16, 2011
 */

char filename[16];
char mapname[60];
char mapinfoname[60];

int Ikon,Ikon2;
int MAP_HEIGHT,MAP_WIDTH;
int MAP_HEIGHT_START,MAP_WIDTH_START,map_tmp;
int Maptype,PencilType;
int Walkable,Decoration;
//FILE* Mapfile;

void MM_DrawHUD(); //Fredrik & Tobias

int MM_ClickHUDButton(int* mPos); //Fredrik & Tobias

void Sub_PencilType(); //Fredrik
void Cleanup(); //Fredrik
void SaveFile(); //Fredrik
void LoadFile(); //Fredrik
void SpawnTile(int *position); //Fredrik
void Pencil(int *position); //Fredrik

int mmInit(); //Fredrik

void mmEvent(SDL_Event* event); //Fredrik
