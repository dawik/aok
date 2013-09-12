/*
 * File:   init.c
 * Author: Projektgrupp 4
 * Created by: Tobias Hultgren
 * Revisioned by: David Wikmans
 *
 * Created on April 2, 2011
 */

#include "header.h"
#include "init.h"
#include "net.h"

void InitVariables()
{
    running = true;
    totalUnits = -1;
}

void InitClients(struct client_t *cl) {
	int x;
	number_of_clients = 0;
	for (x = 0; x<maxclients; x++) {
		cl[x].udp = 0;
		cl[x].receiving = 0;
		strcpy(cl[x].name," ");
	}
}
void LoadClasses()
{
	int i;
	char temp[30];
	FILE *file = fopen("cfg/units.dat", "rt");
	fscanf(file, "%s %d", temp, &UNIT_CLASSES);
	for (i = 1; i <= UNIT_CLASSES; i++)
	{
		fscanf(file, "%s", temp);
		fscanf(file, "%s %f", temp, &unit_class[i].width);
		fscanf(file, "%s %f", temp, &unit_class[i].height);
		fscanf(file, "%s %d", temp, &unit_class[i].speed);
		fscanf(file, "%s %d", temp, &unit_class[i].health);
		fscanf(file, "%s %d", temp, &unit_class[i].damage);
		fscanf(file, "%s %d", temp, &unit_class[i].range);
		fscanf(file, "%s %d", temp, &unit_class[i].attackspeed);
		fscanf(file, "%s %d", temp, &unit_class[i].sight);
		fscanf(file, "%s %d", temp, &unit_class[i].canwork);
		fscanf(file, "%s %d", temp, &unit_class[i].price);
	}
	fclose(file);
}

int LoadMap(char *filename)
{
total_spawnpoints = 0;
int x,y,i;
char pathname[32];
snprintf(pathname,32,"maps/%s.dat",filename);
FILE* Mapfile;
	Mapfile=fopen(pathname,"rb");
	if(Mapfile != NULL){
    fread(&MAP_HEIGHT,sizeof(int),1,Mapfile);
    fread(&MAP_WIDTH,sizeof(int),1,Mapfile);
    fread(&total_spawnpoints,sizeof(int),1,Mapfile);
    for(i=0; i<total_spawnpoints; i++)
    {
      fread(&x,sizeof(int),1,Mapfile);
      fread(&y,sizeof(int),1,Mapfile);
      map[x][y].decoration = 100;
      playerStart[i].x = x;
      playerStart[i].y = y;
    }
		for(x=0;x<MAP_WIDTH;x++)
			for(y=0;y<MAP_HEIGHT;y++)
			{
				fread(&map[x][y].background,sizeof(int),1,Mapfile);
				fread(&map[x][y].walkable,sizeof(int),1,Mapfile);
				fread(&map[x][y].decoration,sizeof(int),1,Mapfile);
				map[x][y].unit=NULL;
			}
	} else {
		fprintf(stderr,"LoadMap: No such file\n");
		if (running == 0)
			exit(1);
		else
			return(1);
	}

	fclose(Mapfile);
	return 0;
}

void ResetWorld()
{
	int x,y;
	for (x=0; x < MAP_WIDTH; x++) {
		for (y=0; y < MAP_HEIGHT; y++) {
			map[x][y].unit = NULL;
		}
	}
	for (x=0; x < totalUnits; x++) {
		unit[x].class = 0;
	}
	totalUnits = 0;
	if (DEBUG) fprintf(stdout,"Map and units reset\n");
}

void LoadSettings()
{
	char temp[30];
	FILE *file = fopen("cfg/settings.cfg", "rt");
	if(file == NULL){
		fprintf(stderr, "##### LoadSettings: Can't load settings file!!");
		exit(43);
	}
	while(!feof(file))
	{
		fscanf(file, "%s", temp);
		if(strcmp( temp,"pathdistance:" ) == 0 ) fscanf(file, "%d", &MAX_CLICKLENGTH);
	}
	fclose(file);
	fprintf(stdout,"##### Loaded settings from server:\nPathdistance: %d\n", MAX_CLICKLENGTH);
}

int Init()
{
	srand(time(0));

	InitVariables();
	LoadSettings();
	LoadClasses();
	ResetWorld();

	int i;
	for(i=0; i<MAX_UNITS; i++)
	{
		unit[i].class = 0;
		unit[i].owner = 9999;
		unitlock[i] = SDL_CreateMutex();
	}

	return 0;
}
