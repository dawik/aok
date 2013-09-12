/*
 * File:   mapmaker.c
 * Author: Projektgrupp 4
 * Created by: Fredrik Berg
 * Revisioned by:
 *
 * Created on May 16, 2011
 */

#include "header.h"
#include "mapmaker.h"
#include "render.h"
FILE *Mapfile;

void MapMaker()
{
//    freopen("CON","w",stdout);
//  printf("Ange Map-Namn:  ");
//  scanf("%s",filename);
//  gets(filename);
//    if(Init() == false) return -1;
    int x, y;
    MAP_WIDTH=100;
    MAP_HEIGHT=100;
    MAP_HEIGHT_START=100;
    MAP_WIDTH_START=100;
    map_tmp=-1;
    PencilType=1;
    Decoration=0;
//    strcpy(filename,argv[1]);
//    fprintf(stderr,"%s",argv[1]);
//    fprintf(stderr,"main: %s\n",filename);
    for(x=0;x<MAP_WIDTH;x++)
        for(y=0;y<MAP_HEIGHT;y++)
        {
            map[x][y].background = 1;
            map[x][y].walkable = 1;
            map[x][y].decoration = 0;
            map[x][y].seen = 1;
        }
//    SDL_Delay(5000);
}

/*Spawn the tile*/
void SpawnTile(int *position)
{
  switch(Maptype)
  {
    case 1: //Grass
      switch(PencilType)
      {
        case 1: Pencil(position); break;
        case 2: Pencil(position); break;
        case 3: Pencil(position); break;
        case 4: Pencil(position); break;
      }
      break;
    case 2: //Vatten
      switch(PencilType)
      {
        case 1: Pencil(position); break;
        case 2: Pencil(position); break;
        case 3: Pencil(position); break;
        case 4: Pencil(position); break;
      }
      break;
    case 3: //Berg
      switch(PencilType)
      {
        case 1: Pencil(position); break;
        case 2: Pencil(position); break;
        case 3: Pencil(position); break;
        case 4: Pencil(position); break;
      }
      break;
    case 4: //Trees
      switch(PencilType)
      {
        int a,b;
        case 1: Pencil(position);
          if(position[0] < 0) return;
          map[position[0]][position[1]].decoration = rand()%3+1;
          map[position[0]][position[1]].walkable = Walkable;
          break;
        case 2: Pencil(position);
          for(a=-1; a<=1; a++)
            for(b=-1; b<=1; b++)
            {
              if(position[0]+b < 0) continue;
              map[position[0]+b][position[1]+a].decoration = rand()%3+1;
              map[position[0]+b][position[1]+a].walkable = Walkable;
            }
          break;
        case 3: Pencil(position);
        for(a=-2; a<=2; a++)
            for(b=-2; b<=2; b++)
            {
              if(position[0]+b < 0) continue;
              map[position[0]+b][position[1]+a].decoration = rand()%3+1;
              map[position[0]+b][position[1]+a].walkable = Walkable;
            }
        break;
        case 4: Pencil(position);
          for(a=-3; a<=3; a++)
            for(b=-3; b<=3; b++)
            {
              if(position[0]+b < 0) continue;
              map[position[0]+b][position[1]+a].decoration = rand()%3+1;
              map[position[0]+b][position[1]+a].walkable = Walkable;
            }
          break;
      }
      break;
    case 5: //Sand
      switch(PencilType)
      {
        case 1: Pencil(position); break;
        case 2: Pencil(position); break;
        case 3: Pencil(position); break;
        case 4: Pencil(position); break;
      }
      break;
    case 6://Minerals
      switch(PencilType)
      {
        int a,b;
        case 1: Pencil(position);
          if(position[0] < 0) return;
          map[position[0]][position[1]].decoration = Decoration;
          map[position[0]][position[1]].walkable = Walkable;
          break;
        case 2: Pencil(position);
          for(a=-1; a<=1; a++)
            for(b=-1; b<=1; b++)
            {
              if(position[0]+b < 0) continue;
              map[position[0]+b][position[1]+a].decoration = Decoration;
              map[position[0]+b][position[1]+a].walkable = Walkable;
            }
          break;
        case 3: Pencil(position);
        for(a=-2; a<=2; a++)
            for(b=-2; b<=2; b++)
            {
              if(position[0]+b < 0) continue;
              map[position[0]+b][position[1]+a].decoration = Decoration;
              map[position[0]+b][position[1]+a].walkable = Walkable;
            }
        break;
        case 4: Pencil(position);
          for(a=-3; a<=3; a++)
            for(b=-3; b<=3; b++)
            {
              if(position[0]+b < 0) continue;
              map[position[0]+b][position[1]+a].decoration = Decoration;
              map[position[0]+b][position[1]+a].walkable = Walkable;
            }
          break;
      }
      break;
    case 7://StartPos
      switch(PencilType)
      {
        int a,b;
        case 1: Pencil(position);
          if(position[0] < 0) return;
          map[position[0]][position[1]].decoration = Decoration;
          map[position[0]][position[1]].walkable = Walkable;
          break;
        case 2: Pencil(position);
          for(a=-1; a<=1; a++)
            for(b=-1; b<=1; b++)
            {
              if(position[0]+b < 0) continue;
              map[position[0]+b][position[1]+a].decoration = Decoration;
              map[position[0]+b][position[1]+a].walkable = Walkable;
            }
          break;
        case 3: Pencil(position);
        for(a=-2; a<=2; a++)
            for(b=-2; b<=2; b++)
            {
              if(position[0]+b < 0) continue;
              map[position[0]+b][position[1]+a].decoration = Decoration;
              map[position[0]+b][position[1]+a].walkable = Walkable;
            }
        break;
        case 4: Pencil(position);
          for(a=-3; a<=3; a++)
            for(b=-3; b<=3; b++)
            {
              if(position[0]+b < 0) continue;
              map[position[0]+b][position[1]+a].decoration = Decoration;
              map[position[0]+b][position[1]+a].walkable = Walkable;
            }
          break;
      }
      break;
  }
}

/*sets the pencilarea*/
void Pencil(int *position)
{
  int a,b;
  switch(PencilType)
  {
    case 1://1 Tile
      GetIsometricPoint(position);
      if(Maptype == 1 || Maptype == 2 || Maptype == 3 || Maptype == 5)
      {
        if(position[0] < 0) return;
        map[position[0]][position[1]].background = Maptype;
        map[position[0]][position[1]].walkable = Walkable;
        map[position[0]][position[1]].decoration = Decoration;
      }
      else
      {
        if(position[0] < 0) return;
        map[position[0]][position[1]].walkable = Walkable;
        map[position[0]][position[1]].decoration = Decoration;
      }
      break;

    case 2://3x3 Tiles
      GetIsometricPoint(position);
      for(a=-1; a<=1; a++)
        for(b=-1; b<=1; b++)
        {
          if(Maptype == 1 || Maptype == 2 || Maptype == 3 || Maptype == 5)
          {
            if(position[0]+b < 0) continue;
            map[position[0]+b][position[1]+a].background = Maptype;
            map[position[0]+b][position[1]+a].walkable = Walkable;
            map[position[0]+b][position[1]+a].decoration = Decoration;
          }
          else
          {
            if(position[0]+b < 0) continue;
            map[position[0]+b][position[1]+a].walkable = Walkable;
            map[position[0]+b][position[1]+a].decoration = Decoration;
          }
        }
      break;
    case 3://5x5 Tiles
      GetIsometricPoint(position);
      for(a=-2; a<=2; a++)
        for(b=-2; b<=2; b++)
        {
          if(Maptype == 1 || Maptype == 2 || Maptype == 3 || Maptype == 5)
          {
            if(position[0]+b < 0) continue;
            map[position[0]+b][position[1]+a].background = Maptype;
            map[position[0]+b][position[1]+a].walkable = Walkable;
            map[position[0]+b][position[1]+a].decoration = Decoration;
          }
          else
          {
            if(position[0]+b < 0) continue;
            map[position[0]+b][position[1]+a].walkable = Walkable;
            map[position[0]+b][position[1]+a].decoration = Decoration;
          }
        }
      break;
    case 4://7x7 Tiles
      GetIsometricPoint(position);
      for(a=-3; a<=3; a++)
        for(b=-3; b<=3; b++)
        {
          if(Maptype == 1 || Maptype == 2 || Maptype == 3 || Maptype == 5)
          {
            if(position[0]+b < 0) continue;
            map[position[0]+b][position[1]+a].background = Maptype;
            map[position[0]+b][position[1]+a].walkable = Walkable;
            map[position[0]+b][position[1]+a].decoration = Decoration;
          }
          else
          {
            if(position[0]+b < 0) continue;
            map[position[0]+b][position[1]+a].walkable = Walkable;
            map[position[0]+b][position[1]+a].decoration = Decoration;
          }
        }
      break;
  }
}

/*Increases the Penciltype with 1*/
void Add_PencilType()
{
  if(PencilType>=1 && PencilType<=3)
    PencilType++;
  else
    PencilType+=0;
}

/*Decrease the penciltype with 1*/
void Sub_PencilType()
{
  if(PencilType>=2 && PencilType<=4)
    PencilType--;
  else
    PencilType+=0;
}

void Rand_Trees()
{
    int x,y;
      for(x=0;x<MAP_WIDTH; x++)
        for(y=0;y<MAP_HEIGHT;y++)
            if(map[x][y].background == 1 && rand()%20==18 && map[x][y].decoration == 0)
            {
              map[x][y].walkable = Walkable;
              map[x][y].decoration = rand()%3+1;
            }
}

/*sets the randomized tree decoration value*/
int Rand_SingleTree()
{
  int Decoration;
  Decoration=rand()%3+1;
  return Decoration;
}

/*Increases the map with 1 row in height and width*/
void Inc_Map()
{
  if(MAP_HEIGHT < MAP_HEIGHT_MAX && MAP_WIDTH < MAP_WIDTH_MAX)
  {
    MAP_HEIGHT++;
    MAP_WIDTH++;
    map_tmp++;
  }
  else
  {
    MAP_HEIGHT+=0;
    MAP_WIDTH+=0;
    map_tmp+=0;
  }
}

/*Decreases the map with 1 row in height and width*/
void Dec_Map()
{
  if(MAP_HEIGHT > 10 && MAP_WIDTH > 10)
  {
    MAP_HEIGHT--;
    MAP_WIDTH--;
    map_tmp--;
  }
  else
  {
    MAP_HEIGHT-=0;
    MAP_WIDTH-=0;
    map_tmp-=0;
  }
}

/*Makes it possible to scale the map, draws new backgrounds for the
exapanded area*/
void PrintMap()
{
  int x, y;
  for(x=MAP_WIDTH_START+map_tmp; x<MAP_WIDTH; x++)
    for(y=0; y<MAP_HEIGHT; y++)
    {
      map[x][y].background = 1;
      map[x][y].walkable = 1;
      map[x][y].decoration = 0;
      map[x][y].seen = 1;
    }
  for(x=0;x<MAP_WIDTH;x++)
    for(y=MAP_HEIGHT_START+map_tmp;y<MAP_HEIGHT;y++)
    {
      map[x][y].background = 1;
      map[x][y].walkable = 1;
      map[x][y].decoration = 0;
      map[x][y].seen = 1;
    }
}

/*Clears the map to its original look, size does not change*/
void ClearMap()
{
  int x,y;

  for(x=0;x<MAP_WIDTH;x++)
    for(y=0;y<MAP_HEIGHT;y++)
    {
      map[x][y].background = 1;
      map[x][y].walkable = 1;
      map[x][y].decoration = 0;
      map[x][y].seen = 1;
    }
}

void kolla()
{
  int x,y;
  for(x=0;x<MAP_WIDTH; x++)
    for(y=0;y<MAP_HEIGHT;y++)
    {
      fprintf(stderr,"[x][y]background: %d %d %d\n",x,y,map[x][y].walkable);
      fprintf(stderr,"[x][y]decoration: %d %d %d\n\n",x,y,map[x][x].decoration);
    }
}

/*Saves the binary file*/
void SaveFile()
{
  int x,y,Player=0;

  for(x=0;x<MAP_WIDTH;x++)
    for(y=0;y<MAP_HEIGHT;y++)
      if(map[x][y].decoration==100)
        Player++;

  fwrite(&MAP_HEIGHT,sizeof(int),1,Mapfile);
  fwrite(&MAP_WIDTH,sizeof(int),1,Mapfile);
  fwrite(&Player,sizeof(int),1,Mapfile);
  for(x=0;x<MAP_WIDTH;x++)
    for(y=0;y<MAP_HEIGHT;y++)
      if(map[x][y].decoration==100)
      {
        fwrite(&x,sizeof(int),1,Mapfile);
        fwrite(&y,sizeof(int),1,Mapfile);
      }

  for(x=0;x<MAP_WIDTH;x++)
    for(y=0;y<MAP_HEIGHT;y++)
    {
      fwrite(&map[x][y].background,sizeof(int),1,Mapfile);
      fwrite(&map[x][y].walkable,sizeof(int),1,Mapfile);
      fwrite(&map[x][y].decoration,sizeof(int),1,Mapfile);
    }
}

/*Loads the binary map file*/
void LoadFile()
{
  int x,y,Player=0,i;

  fread(&MAP_HEIGHT,sizeof(int),1,Mapfile);
  fread(&MAP_WIDTH,sizeof(int),1,Mapfile);
  fread(&Player,sizeof(int),1,Mapfile);

  for(i=0; i<Player; i++)
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
    }
}

/*Handles the events for the MapMaker*/
void mmEvent(SDL_Event* event)
{
    int mousePosition[2];
    switch (event->type)
    {
        case SDL_QUIT:
            running = false;
            break;

        case SDL_KEYDOWN:
        {
            switch (event->key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    mapmakerOpen = false;
                    break;
                case SDLK_LEFT:
                    offsetflag[0]=-1;
                    break;
                case SDLK_RIGHT:
                    offsetflag[0]=1;
                    break;
                case SDLK_DOWN:
                    offsetflag[1]=1;
                    break;
                case SDLK_UP:
                    offsetflag[1]=-1;
                    break;
                case SDLK_1: //Gräs
                    Maptype=1;
                    Walkable=1;
                    Decoration=0;
                    Ikon=1;
                    break;
                case SDLK_2: //Vatten
                    Maptype=2;
                    Walkable=0;
                    Decoration=0;
                    Ikon=2;
                    break;
                case SDLK_3: //Berg
                    Maptype=3;
                    Walkable=0;
                    Decoration=0;
                    Ikon=3;
                    break;
                case SDLK_4: //Sand
                    Maptype=5;
                    Walkable=1;
                    Decoration=0;
                    Ikon=4;
                    break;
                case SDLK_5: //Träd
                    Maptype=4;
                    Walkable=0;
                    Ikon=5;
                    break;
                case SDLK_6://Mineraler
                    Maptype=6;
                    Walkable=0;
                    Decoration=4;
                    Ikon=6;
                    break;
                case SDLK_7://Startpositioner
                    Maptype=7;
                    Walkable=1;
                    Decoration=100;
                    Ikon=7;
                    break;
                case SDLK_8://spara fil
                    Mapfile=fopen("noname.dat","wb");
                    SaveFile();
                    fclose(Mapfile);
                    Ikon=8;
                    break;
                case SDLK_9: //ladda fil
                    Mapfile=fopen("noname.dat","rb");
                    LoadFile();
                    fclose(Mapfile);
                    Ikon=9;
                    break;
                case SDLK_0://"Rensar kartan"
                    ClearMap();
                    Ikon=10;
                    break;
                case SDLK_q://Skalar ner mapen
                    Dec_Map();
                    PrintMap();
                    Ikon=11;
                    break;
                case SDLK_w://Skalar upp mapen
                    Inc_Map();
                    PrintMap();
                    Ikon=12;
                    break;
                case SDLK_e://Pensel 1 (Vanlig)
                    Sub_PencilType();
                    Ikon2=1;
                    break;
                case SDLK_r://Pensel 2 (1 tile runtom den vanliga)
                    Add_PencilType();
                    Ikon2=2;
                    break;
                case SDLK_k://Slumpar ut träd på banan där det är gräs
                    Maptype=4;
                    Walkable=0;
                    Rand_Trees();
                    break;
                default:
                    break;
            }
            break;
        }
        case SDL_KEYUP:
            switch (event->key.keysym.sym)
            {
                case SDLK_LEFT:
                    if(offsetflag[0] == -1) offsetflag[0]=0;
                    break;
                case SDLK_RIGHT:
                    if(offsetflag[0] == 1) offsetflag[0]=0;
                    break;
                case SDLK_DOWN:
                    if(offsetflag[1] == 1) offsetflag[1]=0;
                    break;
                case SDLK_UP:
                    if(offsetflag[1] == -1) offsetflag[1]=0;
                    break;
                default:
                    break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
        {
          case SDL_MOUSEMOTION:
            switch (event->button.button)
            {
                case SDL_BUTTON_LEFT:
                    SDL_GetMouseState(&mousePosition[0],&mousePosition[1]);
                    if(mousePosition[1] < WINDOWHEIGHT-HUD_HEIGHT) //If the mouse is within the game area
                    {
                      switch(Maptype)
                      {
                        case 1:
                          SpawnTile(mousePosition);
                          break;
                        case 2:
                          SpawnTile(mousePosition);
                          break;
                        case 3:
                          SpawnTile(mousePosition);
                          break;
                        case 4:
                          SpawnTile(mousePosition);
                          Decoration=Rand_SingleTree();
                          break;
                        case 5:
                          SpawnTile(mousePosition);
                          break;
                        case 6:
                          SpawnTile(mousePosition);
                          break;
                        case 7:
                          SpawnTile(mousePosition);
                          break;
                        default:
                          break;
                      }
                    }
                    else
                    {
                        hudClick = true;
                        MM_ClickHUDButton(mousePosition);
                    }
                  break;
                case SDL_BUTTON_WHEELUP:
                    zoomSchedueled += 1;
                    break;
                case SDL_BUTTON_WHEELDOWN:
                    zoomSchedueled -= 1;
                    break;
                default:
                    return;
                    break;
            }
            break;
        }
        break;
    }
    return;
}

/*Draws the HUD for the Mapmaker*/
void MM_DrawHUD()
{
    int i;

    glColor3f(0.2, 0.2, 0.2); //Draws main window
    glBegin(GL_QUADS);
    glVertex3f(0+offset[0]*TILEDIAGONAL, WINDOWHEIGHT-HUD_HEIGHT+offset[1]*TILEDIAGONAL/2, 0);
    glVertex3f(WINDOWWIDTH+offset[0]*TILEDIAGONAL, WINDOWHEIGHT-HUD_HEIGHT+offset[1]*TILEDIAGONAL/2, 0);
    glVertex3f(WINDOWWIDTH+offset[0]*TILEDIAGONAL, WINDOWHEIGHT+offset[1]*TILEDIAGONAL/2, 0);
    glVertex3f(0+offset[0]*TILEDIAGONAL, WINDOWHEIGHT+offset[1]*TILEDIAGONAL/2, 0);
    glEnd();

    glColor3f(0.5, 0.5, 0.5); //Draws top line
    glBegin(GL_QUADS);
    glVertex3f(0+offset[0]*TILEDIAGONAL, WINDOWHEIGHT-HUD_HEIGHT-HUD_LINE_HEIGHT+offset[1]*TILEDIAGONAL/2, 0);
    glVertex3f(WINDOWWIDTH+offset[0]*TILEDIAGONAL, WINDOWHEIGHT-HUD_HEIGHT-HUD_LINE_HEIGHT+offset[1]*TILEDIAGONAL/2, 0);
    glVertex3f(WINDOWWIDTH+offset[0]*TILEDIAGONAL, WINDOWHEIGHT-HUD_HEIGHT+offset[1]*TILEDIAGONAL/2, 0);
    glVertex3f(0+offset[0]*TILEDIAGONAL, WINDOWHEIGHT-HUD_HEIGHT+offset[1]*TILEDIAGONAL/2, 0);
    glEnd();

    glColor3f(1.0, 1.0, 1.0); //Draws HUD icons
    for(i = 0; i < MAX_TEXTURES_MMHUD - 4; i++)
        DrawRectangle(1, texture.mmhud[i],
                      WINDOWWIDTH - (i+1)*(HUD_HEIGHT + HUD_ICON_PADDING), //X position
                      WINDOWHEIGHT - HUD_HEIGHT + HUD_ICON_PADDING, //Y position
                      HUD_HEIGHT - 2 * HUD_ICON_PADDING, //X size
                      HUD_HEIGHT - 2 * HUD_ICON_PADDING); //Y size
    switch(Ikon)
    {
      case 1:
        DrawRectangle(1, texture.mmhud[14],
                      WINDOWWIDTH - (16+1)*(HUD_HEIGHT + HUD_ICON_PADDING), //X position
                      WINDOWHEIGHT - HUD_HEIGHT + HUD_ICON_PADDING, //Y position
                      HUD_HEIGHT - 2 * HUD_ICON_PADDING, //X size
                      HUD_HEIGHT - 2 * HUD_ICON_PADDING); //Y size
        break;
      case 2:
        DrawRectangle(1, texture.mmhud[13],
                      WINDOWWIDTH - (16+1)*(HUD_HEIGHT + HUD_ICON_PADDING), //X position
                      WINDOWHEIGHT - HUD_HEIGHT + HUD_ICON_PADDING, //Y position
                      HUD_HEIGHT - 2 * HUD_ICON_PADDING, //X size
                      HUD_HEIGHT - 2 * HUD_ICON_PADDING); //Y size
        break;
      case 3:
        DrawRectangle(1, texture.mmhud[12],
                      WINDOWWIDTH - (16+1)*(HUD_HEIGHT + HUD_ICON_PADDING), //X position
                      WINDOWHEIGHT - HUD_HEIGHT + HUD_ICON_PADDING, //Y position
                      HUD_HEIGHT - 2 * HUD_ICON_PADDING, //X size
                      HUD_HEIGHT - 2 * HUD_ICON_PADDING); //Y size
        break;
      case 4:
        DrawRectangle(1, texture.mmhud[11],
                      WINDOWWIDTH - (16+1)*(HUD_HEIGHT + HUD_ICON_PADDING), //X position
                      WINDOWHEIGHT - HUD_HEIGHT + HUD_ICON_PADDING, //Y position
                      HUD_HEIGHT - 2 * HUD_ICON_PADDING, //X size
                      HUD_HEIGHT - 2 * HUD_ICON_PADDING); //Y size
        break;
      case 5:
        DrawRectangle(1, texture.mmhud[10],
                      WINDOWWIDTH - (16+1)*(HUD_HEIGHT + HUD_ICON_PADDING), //X position
                      WINDOWHEIGHT - HUD_HEIGHT + HUD_ICON_PADDING, //Y position
                      HUD_HEIGHT - 2 * HUD_ICON_PADDING, //X size
                      HUD_HEIGHT - 2 * HUD_ICON_PADDING); //Y size
        break;
      case 6:
        DrawRectangle(1, texture.mmhud[9],
                      WINDOWWIDTH - (16+1)*(HUD_HEIGHT + HUD_ICON_PADDING), //X position
                      WINDOWHEIGHT - HUD_HEIGHT + HUD_ICON_PADDING, //Y position
                      HUD_HEIGHT - 2 * HUD_ICON_PADDING, //X size
                      HUD_HEIGHT - 2 * HUD_ICON_PADDING); //Y size
        break;
      case 7:
        DrawRectangle(1, texture.mmhud[8],
                      WINDOWWIDTH - (16+1)*(HUD_HEIGHT + HUD_ICON_PADDING), //X position
                      WINDOWHEIGHT - HUD_HEIGHT + HUD_ICON_PADDING, //Y position
                      HUD_HEIGHT - 2 * HUD_ICON_PADDING, //X size
                      HUD_HEIGHT - 2 * HUD_ICON_PADDING); //Y size
        break;
    }
    if(Ikon2 == 1)
      switch(PencilType)
      {
        case 1:
            DrawRectangle(1, texture.mmhud[15],
                          WINDOWWIDTH - (15+1)*(HUD_HEIGHT + HUD_ICON_PADDING), //X position
                          WINDOWHEIGHT - HUD_HEIGHT + HUD_ICON_PADDING, //Y position
                          HUD_HEIGHT - 2 * HUD_ICON_PADDING, //X size
                          HUD_HEIGHT - 2 * HUD_ICON_PADDING); //Y size
            break;
        case 2:
            DrawRectangle(1, texture.mmhud[16],
                          WINDOWWIDTH - (15+1)*(HUD_HEIGHT + HUD_ICON_PADDING), //X position
                          WINDOWHEIGHT - HUD_HEIGHT + HUD_ICON_PADDING, //Y position
                          HUD_HEIGHT - 2 * HUD_ICON_PADDING, //X size
                          HUD_HEIGHT - 2 * HUD_ICON_PADDING); //Y size
            break;
        case 3:
            DrawRectangle(1, texture.mmhud[17],
                          WINDOWWIDTH - (15+1)*(HUD_HEIGHT + HUD_ICON_PADDING), //X position
                          WINDOWHEIGHT - HUD_HEIGHT + HUD_ICON_PADDING, //Y position
                          HUD_HEIGHT - 2 * HUD_ICON_PADDING, //X size
                          HUD_HEIGHT - 2 * HUD_ICON_PADDING); //Y size
            break;
        case 4:
            DrawRectangle(1, texture.mmhud[18],
                          WINDOWWIDTH - (15+1)*(HUD_HEIGHT + HUD_ICON_PADDING), //X position
                          WINDOWHEIGHT - HUD_HEIGHT + HUD_ICON_PADDING, //Y position
                          HUD_HEIGHT - 2 * HUD_ICON_PADDING, //X size
                          HUD_HEIGHT - 2 * HUD_ICON_PADDING); //Y size
            break;
      }
    else
      switch(PencilType)
      {
        case 1:
            DrawRectangle(1, texture.mmhud[15],
                          WINDOWWIDTH - (15+1)*(HUD_HEIGHT + HUD_ICON_PADDING), //X position
                          WINDOWHEIGHT - HUD_HEIGHT + HUD_ICON_PADDING, //Y position
                          HUD_HEIGHT - 2 * HUD_ICON_PADDING, //X size
                          HUD_HEIGHT - 2 * HUD_ICON_PADDING); //Y size
            break;
        case 2:
            DrawRectangle(1, texture.mmhud[16],
                          WINDOWWIDTH - (15+1)*(HUD_HEIGHT + HUD_ICON_PADDING), //X position
                          WINDOWHEIGHT - HUD_HEIGHT + HUD_ICON_PADDING, //Y position
                          HUD_HEIGHT - 2 * HUD_ICON_PADDING, //X size
                          HUD_HEIGHT - 2 * HUD_ICON_PADDING); //Y size
            break;
        case 3:
            DrawRectangle(1, texture.mmhud[17],
                          WINDOWWIDTH - (15+1)*(HUD_HEIGHT + HUD_ICON_PADDING), //X position
                          WINDOWHEIGHT - HUD_HEIGHT + HUD_ICON_PADDING, //Y position
                          HUD_HEIGHT - 2 * HUD_ICON_PADDING, //X size
                          HUD_HEIGHT - 2 * HUD_ICON_PADDING); //Y size
            break;
        case 4:
            DrawRectangle(1, texture.mmhud[18],
                          WINDOWWIDTH - (15+1)*(HUD_HEIGHT + HUD_ICON_PADDING), //X position
                          WINDOWHEIGHT - HUD_HEIGHT + HUD_ICON_PADDING, //Y position
                          HUD_HEIGHT - 2 * HUD_ICON_PADDING, //X size
                          HUD_HEIGHT - 2 * HUD_ICON_PADDING); //Y size
            break;
      }

}

/*Checks which ikon had been clicked and what action to take*/
int MM_ClickHUDButton(int* mPos)
{
	int i;
	for(i = 0; i < MAX_TEXTURES_MMHUD; i++)
	{
		if(    mPos[0] > WINDOWWIDTH - (i+1)*(HUD_HEIGHT + HUD_ICON_PADDING)
				&& mPos[0] < WINDOWWIDTH - (i+1)*(HUD_HEIGHT + HUD_ICON_PADDING) + HUD_HEIGHT - HUD_ICON_PADDING
				&& mPos[1] > WINDOWHEIGHT - HUD_HEIGHT + HUD_ICON_PADDING
				&& mPos[1] < WINDOWHEIGHT - HUD_ICON_PADDING)
		{
			switch(i)
			{
				case 14:
          Maptype=1;
          Walkable=1;
          Decoration=0;
          Ikon=1;
					break;
				case 13:
          Maptype=2;
          Walkable=0;
          Decoration=0;
          Ikon=2;
					break;
				case 12:
          Maptype=3;
          Walkable=0;
          Decoration=0;
          Ikon=3;
					break;
        case 11:
          Maptype=5;
          Walkable=1;
          Decoration=0;
          Ikon=4;
					break;
				case 10:
          Maptype=4;
          Walkable=0;
          Ikon=5;
					break;
        case 9:
          Maptype=6;
          Walkable=0;
          Decoration=4;
          Ikon=6;
          break;
        case 8:
          Maptype=7;
          Walkable=1;
          Decoration=100;
          Ikon=7;
          break;
				case 7:
          Mapfile=fopen("noname.dat","wb");
          SaveFile();
          fclose(Mapfile);
          Ikon=8;
					break;
				case 6:
          Mapfile=fopen("noname.dat","rb");
          LoadFile();
          fclose(Mapfile);
          Ikon=9;
					break;
				case 5:
          ClearMap();
          Ikon=10;
					break;
				case 4:
          Dec_Map();
          PrintMap();
          Ikon=11;
					break;
				case 3:
          Inc_Map();
          PrintMap();
          Ikon=12;
					break;
				case 2:
          Sub_PencilType();
          Ikon2=1;
					break;
				case 1:
          Add_PencilType();
          Ikon2=2;
					break;
				case 0:
          mapmakerOpen = false;
					break;
			}
			return i;
		}
	}
	return false;
}
