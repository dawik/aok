/*
 * File:   init.c
 * Author: Projektgrupp 4
 * Created by: Tobias Hultgren
 * Revisioned by:
 *
 * Created on April 2, 2011
 */

#include "header.h"
#include "init.h"
#include "sound.h"

void NewSettings()
{
    FILE *file = fopen("cfg/settings.cfg", "wt");
    if(file == NULL){
        fprintf(stderr, "NewSettings: Can't create new settings file\n");
        exit(7);
    }

    fprintf(file, "//Multiplayer\n");
    fprintf(file, "username: player\n");
    fprintf(file, "server: sthd4.no-ip.org\n");
    fprintf(file, "port: 6001\n");

    fprintf(file, "\n//Settings\n");
    fprintf(file, "resolution: 640 480\n");
    fprintf(file, "framerate: 60\n");
    fprintf(file, "interpolation: 1\n");
    fprintf(file, "textures: 1\n");
    fprintf(file, "fullscreen: 0\n");
    fprintf(file, "volume: 50\n");

    fprintf(file, "\n//Preferences\n");
    fprintf(file, "scrollspeed: 0.3\n");
    fprintf(file, "zoomspeed: 4\n");
    fprintf(file, "fowshader: 0.4\n");
    fprintf(file, "hud_height: 100\n");
    fprintf(file, "hud_line_height: 5\n");
    fprintf(file, "hud_icon_padding: 10\n");
    fprintf(file, "consolerows: 5\n");
    fprintf(file, "boxcolor: 0.2 1.0 0.2\n");
    fprintf(file, "boxborder: 2\n");
    fprintf(file, "playercolor: 0 0.8 0.2 0.2\n");
    fprintf(file, "playercolor: 1 0.2 1.0 0.2\n");
    fprintf(file, "playercolor: 2 0.2 0.2 1.0\n");
    fprintf(file, "playercolor: 3 1.0 1.0 0.0\n");
    fprintf(file, "playercolor: 4 0.0 1.0 1.0\n");
    fprintf(file, "playercolor: 5 1.0 0.0 1.0\n");
    fprintf(file, "playercolor: 6 0.5 0.5 1.0\n");
    fprintf(file, "playercolor: 7 0.0 0.5 0.5\n");

    fclose(file);
}

void LoadSettings()
{
    char temp[30];
    int player, i;
    FILE *file = fopen("cfg/settings.cfg", "rt");
    if(file == NULL){
        fprintf(stderr, "LoadSettings: Can't load settings file, creating new one\n");
        NewSettings();
        file = fopen("cfg/settings.cfg", "rt");
    }
    while(!feof(file))
    {
        fscanf(file, "%s", temp);
        if(strcmp( temp,"resolution:" ) == 0 ) fscanf(file, "%d %d", &WINDOWWIDTH, &WINDOWHEIGHT);
        else if(strcmp( temp,"framerate:" ) == 0 ) fscanf(file, "%d", &FRAMERATE);
        else if(strcmp( temp,"username:" ) == 0 ) fscanf(file, "%s", settings.username);
        else if(strcmp( temp,"server:" ) == 0 ) fscanf(file, "%s", settings.server);
        else if(strcmp( temp,"port:" ) == 0 ) fscanf(file, "%d", &settings.port);
        else if(strcmp( temp,"scrollspeed:" ) == 0 ) fscanf(file, "%f", &SCROLLSPEED);
        else if(strcmp( temp,"zoomspeed:" ) == 0 ) fscanf(file, "%d", &ZOOMSPEED);
        else if(strcmp( temp,"interpolation:" ) == 0 ) fscanf(file, "%d", &settings.interpolation);
        else if(strcmp( temp,"textures:" ) == 0 ) fscanf(file, "%d", &settings.textures);
        else if(strcmp( temp,"fullscreen:" ) == 0 ) fscanf(file, "%d", &settings.fullscreen);
        else if(strcmp( temp,"volume:" ) == 0 ) fscanf(file, "%d", &settings.volume);
        else if(strcmp( temp,"fowshader:" ) == 0 ) fscanf(file, "%f", &settings.shader);
        else if(strcmp( temp,"consolerows:" ) == 0 ) fscanf(file, "%d", &settings.consolerows);
        else if(strcmp( temp,"hud_height:" ) == 0 ) fscanf(file, "%d", &HUD_HEIGHT);
        else if(strcmp( temp,"hud_line_height:" ) == 0 ) fscanf(file, "%d", &HUD_LINE_HEIGHT);
        else if(strcmp( temp,"hud_icon_padding:" ) == 0 ) fscanf(file, "%d", &HUD_ICON_PADDING);
        else if(strcmp( temp,"boxcolor:" ) == 0 ) fscanf(file, "%f %f %f", &settings.boxcolor[0], &settings.boxcolor[1], &settings.boxcolor[2]);
        else if(strcmp( temp,"boxborder:" ) == 0 ) fscanf(file, "%d", &settings.boxborder);
        else if(strcmp( temp,"playercolor:" ) == 0 ) { fscanf(file, "%d", &player); fscanf(file, "%f %f %f", &settings.playercolor[player][0], &settings.playercolor[player][1], &settings.playercolor[player][2]); }
        else;
    }
    fclose(file);
    if(DEBUG) fprintf(stdout,"Loaded settings from file:\nResolution: %dx%d\nFramerate: %d\nUsername: %s\nServer: %s\nPort %d\nSCROLLSPEED: %f\nZOOMSPEED: %d\nInterpolation: %d\nTextures: %d\nFullscreen: %d\nFOW shader: %f\nConsole rows: %d\nVolume: %d\n",
                      WINDOWWIDTH,WINDOWHEIGHT,FRAMERATE,settings.username,settings.server,settings.port,SCROLLSPEED,ZOOMSPEED,settings.interpolation,settings.textures,settings.fullscreen,settings.shader,settings.consolerows,settings.volume);
    if(DEBUG) for(i = 0; i < MAX_CONNECTIONS; i++) fprintf(stdout,"Player %i colors: %f %f %f\n", i, settings.playercolor[i][0], settings.playercolor[i][1], settings.playercolor[i][2]);
}

void InitVariables()
{
    Screen = NULL;
    running = true;
    drawconsole = false;
    shiftDown = false;
    altDown = false;
    ctrlDown = false;
    leftMouseDown = false;
    menuOpen = false;
    mapmakerOpen = false;
    capsActive = false;
    gameVolume = settings.volume;
    musicVolume = settings.volume;
    totalUnits = 0;
    zoomSchedueled = 0;
    resources = 0;
    population = 0;
    countdown = -1;
    endgameStatus = 0;
    showEndgameStatus = false;
    hudClick = false;
    lastSelectedGroup = -1;
    lastAttacked = 0;

    offset[0] = 0;
    offset[1] = 0;

    int i;
    for(i=0; i<MAX_CONNECTIONS; i++) connectedClient[i][0] = '\0';

    TILESIZE = 100;
    TILEDIAGONAL = sqrt(2*pow(TILESIZE,2));
    PACKETSIZE = 1024;
    MESSAGESIZE = 200;
    MAX_POPULATION = 0;
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

void LoadMap()
{
    int x,y,i;
    char fullmapname[70];
    sprintf(fullmapname, "maps/%s.dat",settings.mapname);
    FILE* Mapfile;
    Mapfile=fopen(fullmapname,"rb");
    if(Mapfile != NULL){

        fread(&MAP_HEIGHT,sizeof(int),1,Mapfile);
        fread(&MAP_WIDTH,sizeof(int),1,Mapfile);
        fread(&maxPlayers,sizeof(int),1,Mapfile);

        for(i=0; i<maxPlayers; i++)
        {
          fread(&x,sizeof(int),1,Mapfile);
          fread(&y,sizeof(int),1,Mapfile);
          map[x][y].decoration = 100;
        }
        for(x=0;x<MAP_WIDTH;x++)
            for(y=0;y<MAP_HEIGHT;y++)
            {
                fread(&map[x][y].background,sizeof(int),1,Mapfile);
                fread(&map[x][y].walkable,sizeof(int),1,Mapfile);
                fread(&map[x][y].decoration,sizeof(int),1,Mapfile);
                map[x][y].unit = NULL;
                map[x][y].building = NULL;
                map[x][y].seen = 1;
            }
    } else {
        fprintf(stderr,"Error: Could not load map %s\n", settings.mapname);
        exit(1);
    }
    fclose(Mapfile);

    MAX_POPULATION = 200/maxPlayers;
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
	population = 0;
	if (DEBUG) fprintf(stdout,"Map and units reset\n");
}


void InitSDL()
{
	int i;
	for(i=0; i<MAX_UNITS; i++)
	{
		unit[i].class = 0;
		unitlock[i] = SDL_CreateMutex();
	}

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr,"ERROR: Kunde ej starta SDL!\n");
		return;
	} else if(DEBUG) fprintf(stdout, "Init: Startat upp SDL!\n");

	if(SDLNet_Init() < 0) fprintf(stderr,"Error with function: SDLNet_Init()\n");

	SDL_WM_SetCaption( "Age of KTH Client", NULL );
	SDL_EnableUNICODE(1);
}

void InitAudio()
{
	//Initialize SDL_Mixer
	if(Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096) == -1)
		fprintf(stderr,"Error in function: InitAudio() > Mix_OpenAudio: %s\n",Mix_GetError());

    //Set volumes according to volume in settings.cfg
    Mix_Volume (-1,settings.volume);
    Mix_VolumeMusic (settings.volume);

	//Load sounds and music
	music_sound = Mix_LoadMUS       ( "sound/baddream.ogg" );
	move_sound1 = Mix_LoadWAV       ( "sound/yes.wav" );
	move_sound2 = Mix_LoadWAV       ( "sound/movin.wav" );
	move_sound3 = Mix_LoadWAV       ( "sound/alright.wav" );
	cantwalk_sound = Mix_LoadWAV    ( "sound/cantmovethere.ogg" );
    attack_sound = Mix_LoadWAV      ( "sound/attack1.ogg" );
    attack_sound2 = Mix_LoadWAV     ( "sound/attack2.ogg" );
    attack_sound3 = Mix_LoadWAV     ( "sound/attack3.ogg" );
    follow_sound = Mix_LoadWAV      ( "sound/follow.ogg" );
    underattack_sound = Mix_LoadWAV ( "sound/underattack.ogg" );
    work_sound = Mix_LoadWAV        ( "sound/working.ogg" );
    unitlost_sound = Mix_LoadWAV    ( "sound/unitlost.ogg" );
    win_sound = Mix_LoadWAV         ( "sound/win.ogg" );
    lose_sound = Mix_LoadWAV        ( "sound/lose.ogg" );
    menu_sound = Mix_LoadWAV        ( "sound/menu.ogg" );
    unitmax_sound = Mix_LoadWAV     ( "sound/unitmax.ogg" );

	//Write to stderr if unable to load a sound file.
	if ( music_sound == NULL )          fprintf(stderr,"Error in function: InitAudio() > Mix_LoadWAV(sound/baddream.ogg): %s\n",Mix_GetError());
	if ( menu_sound == NULL )           fprintf(stderr,"Error in function: InitAudio() > Mix_LoadWAV(sound/menu.ogg): %s\n",Mix_GetError());
	if ( move_sound1 == NULL )          fprintf(stderr,"Error in function: InitAudio() > Mix_LoadWAV(sound/yes.ogg): %s\n",Mix_GetError());
	if ( move_sound2 == NULL )          fprintf(stderr,"Error in function: InitAudio() > Mix_LoadWAV(sound/movin.ogg): %s\n",Mix_GetError());
	if ( move_sound3 == NULL )          fprintf(stderr,"Error in function: InitAudio() > Mix_LoadWAV(sound/alright.ogg): %s\n",Mix_GetError());
	if ( cantwalk_sound == NULL )       fprintf(stderr,"Error in function: InitAudio() > Mix_LoadWAV(sound/cantmovethere.ogg): %s\n",Mix_GetError());
    if ( attack_sound == NULL )         fprintf(stderr,"Error in function: InitAudio() > Mix_LoadWAV(sound/attack1.ogg): %s\n",Mix_GetError());
    if ( attack_sound2 == NULL )        fprintf(stderr,"Error in function: InitAudio() > Mix_LoadWAV(sound/attack2.ogg): %s\n",Mix_GetError());
    if ( attack_sound3 == NULL )        fprintf(stderr,"Error in function: InitAudio() > Mix_LoadWAV(sound/attack3.ogg): %s\n",Mix_GetError());
    if ( follow_sound == NULL )         fprintf(stderr,"Error in function: InitAudio() > Mix_LoadWAV(sound/follow.ogg): %s\n",Mix_GetError());
    if ( underattack_sound == NULL )    fprintf(stderr,"Error in function: InitAudio() > Mix_LoadWAV(sound/underattack.ogg): %s\n",Mix_GetError());
    if ( work_sound == NULL )           fprintf(stderr,"Error in function: InitAudio() > Mix_LoadWAV(sound/working.ogg): %s\n",Mix_GetError());
    if ( unitlost_sound == NULL )       fprintf(stderr,"Error in function: InitAudio() > Mix_LoadWAV(sound/unitlost.ogg): %s\n",Mix_GetError());
    if ( win_sound == NULL )            fprintf(stderr,"Error in function: InitAudio() > Mix_LoadWAV(sound/win.ogg): %s\n",Mix_GetError());
    if ( lose_sound == NULL )           fprintf(stderr,"Error in function: InitAudio() > Mix_LoadWAV(sound/lose.ogg): %s\n",Mix_GetError());
    if ( unitmax_sound == NULL )        fprintf(stderr,"Error in function: InitAudio() > Mix_LoadWAV(sound/unitmax.ogg): %s\n",Mix_GetError());
}

int Init()
{
	srand(time(0));

	LoadSettings();
	InitVariables();
	LoadClasses();
	InitSDL();
	InitAudio();

	/*int x, y;
		for(x=0; x<MAP_WIDTH; x++)
		for(y=0; y<MAP_HEIGHT; y++)
		if(map[x][y].background == 1 && rand()%10 > 6){map[x][y].decoration = rand()%3+1; map[x][y].walkable = 0;}
		FILE* Mapfile = fopen("newmap.map","wb");
		for(x=0;x<MAP_WIDTH;x++)
		for(y=0;y<MAP_HEIGHT;y++)
		{
		fwrite(&map[x][y].background,sizeof(int),1,Mapfile);
		fwrite(&map[x][y].walkable,sizeof(int),1,Mapfile);
		fwrite(&map[x][y].decoration,sizeof(int),1,Mapfile);
		}
		fclose(Mapfile);*/

	return 0;
}
