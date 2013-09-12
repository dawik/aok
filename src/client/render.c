/*
 * File:   render.c
 * Author: Projektgrupp 4
 * Created by: Tobias Hultgren
 * Revisioned by:
 *
 * Created on April 2, 2011
 */

#include "header.h"
#include "render.h"
#include "mapmaker.h"

GLuint *ImageToTexture(const char* file)
{
    SDL_Surface* surface = IMG_Load(file);
    GLuint texture_handle;
    glPixelStorei(GL_UNPACK_ALIGNMENT,4);
    glGenTextures(1, &texture_handle);
    glBindTexture(GL_TEXTURE_2D, texture_handle);
    SDL_PixelFormat *format = surface->format;
    if (format->Amask)
    {
        gluBuild2DMipmaps(GL_TEXTURE_2D, 4,
        surface->w, surface->h, GL_RGBA,GL_UNSIGNED_BYTE, surface->pixels);
    }
    else
    {
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
        surface->w, surface->h, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
    }

    SDL_FreeSurface(surface);
    return (GLuint*)texture_handle;
}

void DrawRectangle(int offset_setting,GLuint *textur,float x,float y,int width,int height)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, (GLuint) textur);
    glBegin( GL_QUADS );
    switch(offset_setting)
    {
        case 0:
            glTexCoord2i( 0, 0 );
            glVertex3f( x, y, 0.f);
            glTexCoord2i( 1, 0 );
            glVertex3f( x+width, y, 0.f);
            glTexCoord2i( 1, 1 );
            glVertex3f( x+width, y+height, 0.f );
            glTexCoord2i( 0, 1 );
            glVertex3f( x, y+height, 0.f );
            break;
            break;
        case 1:
            glTexCoord2i( 0, 0 );
            glVertex3f( x+offset[0]*TILEDIAGONAL, y+offset[1]*TILEDIAGONAL/2, 0.f);
            glTexCoord2i( 1, 0 );
            glVertex3f( x+width+offset[0]*TILEDIAGONAL, y+offset[1]*TILEDIAGONAL/2, 0.f);
            glTexCoord2i( 1, 1 );
            glVertex3f( x+width+offset[0]*TILEDIAGONAL, y+height+offset[1]*TILEDIAGONAL/2, 0.f );
            glTexCoord2i( 0, 1 );
            glVertex3f( x+offset[0]*TILEDIAGONAL, y+height+offset[1]*TILEDIAGONAL/2, 0.f );
            break;
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void DrawTextGame(int style, int x, int y, char *text, float *color){
    int i, len = strlen(text);
    glColor3f(color[0], color[1], color[2]);
    glRasterPos2f(x, y);
    for(i=0; i<len; i++){
        switch(style)
        {
            case 0:
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,text[i]);
                break;
            case 1:
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,text[i]);
                break;
            case 2:
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,text[i]);
                break;
            case 3:
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,text[i]);
                break;
            case 4:
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,text[i]);
                break;
            case 5:
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13,text[i]);
                break;
            case 6:
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15,text[i]);
                break;
        }
    }
}

void DrawChat(){
    int i;
    float color[2][3] = {{1.0, 1.0, 1.0},{1.0, 0.5, 0.5}};
    char display_buff[PACKETSIZE];

    sprintf(display_buff, "[%s] %s", settings.username, console);
    DrawTextGame(1,20+offset[0]*TILEDIAGONAL, (offset[1]*TILEDIAGONAL/2)+WINDOWHEIGHT-15, display_buff, drawconsole ? color[1] : color[0]);

    for(i=0; i<settings.consolerows; i++)
    DrawTextGame(1,20+offset[0]*TILEDIAGONAL, (offset[1]*TILEDIAGONAL/2)+WINDOWHEIGHT-30-15*i, messagebuffer[i], color[0]);
}

void DrawMap()
{
    int x, y;
    float isometric[2];

    for(x=0; x<MAP_WIDTH; x++)
        for(y=0; y<MAP_HEIGHT; y++)
        {
            isometric[0] = (x+y+1) * TILEDIAGONAL/2;
            isometric[1] = (x-y-1) * TILEDIAGONAL/4;

            if (  isometric[0] + TILEDIAGONAL - offset[0]*TILEDIAGONAL > 0
               && isometric[0] - TILEDIAGONAL/2 - offset[0]*TILEDIAGONAL < WINDOWWIDTH
               && isometric[1] + TILEDIAGONAL/2 - offset[1]*TILEDIAGONAL/2 > 0
               && isometric[1] - TILEDIAGONAL/4 - offset[1]*TILEDIAGONAL/2 < WINDOWHEIGHT)
            { //Checks if the tile in question is within the screen boundries and only draws it if it is
                if(map[x][y].seen == 1) glColor3f(1.0, 1.0, 1.0);
                else glColor3f(settings.shader, settings.shader, settings.shader); //Draw tiles fully bright only if seen

                if(settings.textures) DrawRectangle(0,texture.tile[map[x][y].background],isometric[0]-1,isometric[1]-1,TILEDIAGONAL+1,TILEDIAGONAL/2+1); //Textures enables
                else //Textures disabled
                {
                    switch(map[x][y].background)
                    {
                        case 1://Grass
                            glColor3f(0.0, 0.9, 0.0);
                            if(DEBUG) if(map[x][y].unit != NULL) glColor3f(0.0, 0.8, 0.0);
                            break;
                        case 2://Water
                            glColor3f(0.0, 0.0, 1.0);
                            break;
                        case 3://Mountains
                            glColor3f(0.3, 0.3, 0.3);
                            break;
                        case 4://Trees
                            glColor3f(0.0, 0.6, 0.0);
                            break;
                        case 5://Sand
                            glColor3f(0.9, 0.6, 0.3);
                            break;
                    }
                    glBegin(GL_QUADS);
                    glVertex3f(isometric[0], isometric[1]+TILEDIAGONAL/4, 0);
                    glVertex3f(isometric[0]+TILEDIAGONAL/2, isometric[1], 0);
                    glVertex3f(isometric[0]+TILEDIAGONAL, isometric[1]+TILEDIAGONAL/4, 0);
                    glVertex3f(isometric[0]+TILEDIAGONAL/2, isometric[1]+TILEDIAGONAL/2, 0);
                    glEnd();
                }
            }
        }
}

void DrawUnits()
{
    int i, x, y, foundresource;
    for(i=0; i<=totalUnits; i++) //Draws seen units within screen boundries
    {
        if(unit[i].class != 0)
        {
            unit[i].destcoord[0] = (unit[i].position[0]+unit[i].position[1]) * TILEDIAGONAL/2 + TILEDIAGONAL; //center of next tile
            unit[i].destcoord[1] = (unit[i].position[0]-unit[i].position[1]) * TILEDIAGONAL/4; //center of next tile
            if(settings.interpolation && (map[unit[i].position[0]][unit[i].position[1]].seen == 1 || unit[i].owner == client_id)) //Interpolate unit position only if unit is either within sight range or is owned by the player, and has the setting to do so
            {
                if(unit[i].fromtime + unit[i].speed > SDL_GetTicks())
                {
                    unit[i].coord[0] = unit[i].fromcoord[0] + (unit[i].destcoord[0] - unit[i].fromcoord[0]) / (unit[i].speed) * (SDL_GetTicks() - unit[i].fromtime);
                    unit[i].coord[1] = unit[i].fromcoord[1] + (unit[i].destcoord[1] - unit[i].fromcoord[1]) / (unit[i].speed) * (SDL_GetTicks() - unit[i].fromtime);
                }
                else
                {
                    unit[i].coord[0] = unit[i].destcoord[0];
                    unit[i].coord[1] = unit[i].destcoord[1];
                }
            }
            else
            {
                unit[i].coord[0] = unit[i].destcoord[0];
                unit[i].coord[1] = unit[i].destcoord[1];
            }

            if (  (unit[i].coord[0] + TILEDIAGONAL - offset[0]*TILEDIAGONAL > 0
               && unit[i].coord[0] - TILEDIAGONAL/2 - offset[0]*TILEDIAGONAL < WINDOWWIDTH
               && unit[i].coord[1] + TILEDIAGONAL/2 - offset[1]*TILEDIAGONAL/2 > 0
               && unit[i].coord[1] - TILEDIAGONAL/4 - offset[1]*TILEDIAGONAL/2 < WINDOWHEIGHT)
               && map[unit[i].position[0]][unit[i].position[1]].seen == 1 ) //Checks if the unit in question is within the screen boundries and is seen by the player, and only draws it if it is
            {
                /*switch(unit[i].class){
                    case 1:
                        glColor3f(1.0, 0.0, 0.0);
                        if (unit[i].selected == true) glColor3f(1.0 - shading, 0.0, 0.0);
                        break;
                }
                glBegin(GL_QUADS);
                glVertex3f(unit[i].coord[0] - unit[i].width*TILEDIAGONAL/2, unit[i].coord[1] - unit[i].height*TILEDIAGONAL, 0);
                glVertex3f(unit[i].coord[0] + unit[i].width*TILEDIAGONAL/2, unit[i].coord[1] - unit[i].height*TILEDIAGONAL, 0);
                glVertex3f(unit[i].coord[0] + unit[i].width*TILEDIAGONAL/2, unit[i].coord[1], 0);
                glVertex3f(unit[i].coord[0] - unit[i].width*TILEDIAGONAL/2, unit[i].coord[1], 0);
                glEnd();*/ //Unit coloring instead of texturing. Possible to implement in the future.

                float drawPosition[2];
                float drawSize[2];
                drawPosition[0] = unit[i].coord[0] - unit[i].width*TILEDIAGONAL/2;
                drawPosition[1] = unit[i].coord[1] - unit[i].height*TILEDIAGONAL/2;
                drawSize[0] = unit[i].width*TILEDIAGONAL;
                drawSize[1] = unit[i].height*TILEDIAGONAL;

                /*if(DEBUG)
                {
                    glColor3f(0.0, 1.0, 0.0);
                    glBegin(GL_QUADS);
                    glVertex3f(drawPosition[0], drawPosition[1], 0);
                    glVertex3f(drawPosition[0]+drawSize[0], drawPosition[1], 0);
                    glVertex3f(drawPosition[0]+drawSize[0], drawPosition[1]+drawSize[1], 0);
                    glVertex3f(drawPosition[0], drawPosition[1]+drawSize[1], 0);
                    glEnd();
                }*/ //Colors the unit frame, not needed at the moment
                glColor3f(settings.playercolor[unit[i].owner][0], settings.playercolor[unit[i].owner][1], settings.playercolor[unit[i].owner][2]);

                /*glPushMatrix();
                glTranslatef(unit[i].coord[0],unit[i].coord[1],0.0);
                glRotatef(unit[i].facing*45, 0.0, 0.0, 1.0);
                DrawRectangle(0, texture.unit[0], - unit[i].width*TILEDIAGONAL/2, - unit[i].height*TILEDIAGONAL/2, drawSize[0], drawSize[1]);
                glPopMatrix();*/ //Unit rotation not being used
                DrawRectangle(0, texture.unit[unit[i].class][unit[i].facing], drawPosition[0], drawPosition[1], drawSize[0], drawSize[1]); //Draws the unit

                if(unit[i].owner == client_id)
                {
                    //Draws status information above the unit if it belongs to the player
                    foundresource = 0;
                    if(unit[i].target == -1) //If unit does not have a target
                    {
                        if(unit[i].canwork == 1) //Check for nearby resources if the unit is of working class
                        {
                            for(x = unit[i].position[0] - 1; x <= unit[i].position[0] + 1; x++)
                                for(y = unit[i].position[1] - 1; y <= unit[i].position[1] + 1; y++)
                                {
                                    if(x < 0 || x >= MAP_WIDTH || y < 0 || x >= MAP_HEIGHT) continue; //Don't look at tiles that do not exist
                                    if(map[x][y].decoration == 4) //Checks if the current tile contains resources
                                    {
                                        foundresource = 1;
                                    }
                                }
                        }
                        if(unit[i].fromtime + unit[i].speed + 100 > SDL_GetTicks()) unit[i].status = MOVING; //Draws moving icon if the unit is moving without a target
                        else if(foundresource == 1) unit[i].status = WORKING; //Change the status to idle if it's next to a resource tile
                        else unit[i].status = IDLE; //Draws idle icon if unit is standing still
                    }
                    else //If unit has a target
                    {
                        if(unit[unit[i].target].class == 0)
                            unit[i].target = -1; //Set the units target to -1 if the target is dead
                        if(unit[unit[i].target].owner == client_id)
                        {
                            unit[i].status = FOLLOWING; //Draws following icon if the unit is targeting a friendly unit, and color the line blue
                            glColor3f(0.0, 0.0, 1.0);
                        }
                        else
                        {
                            unit[i].status = ATTACKING; //Draws attacking icon if the unit is targeting an enemy unit, and color the line red
                            glColor3f(1.0, 0.0, 0.0);
                        }

                        if(altDown || capsActive) DrawLine(unit[i].coord, unit[unit[i].target].coord); //If alt is pressed, draw line to the target
                    }
                    glColor3f(1.0, 1.0, 1.0);
                    if(unit[i].status != IDLE) DrawRectangle(0, texture.status[unit[i].status], drawPosition[0], drawPosition[1] - unit[i].width*TILEDIAGONAL/2 - HEALTHBARSIZE, unit[i].width*TILEDIAGONAL, unit[i].width*TILEDIAGONAL/2); //Draws the status icon
                }

                //Draws health information if alt is down or the unit is selected
                if(altDown || unit[i].selected || capsActive)
                {
                    struct Point start, end;
                    start.x = unit[i].coord[0] - unit[i].width*TILEDIAGONAL/2;
                    start.y = drawPosition[1] - HEALTHBARSIZE;
                    end.x = unit[i].coord[0] + unit[i].width*TILEDIAGONAL/2;
                    end.y = drawPosition[1];

                    glColor3f(0.0, 0.0, 0.0); //Draws box around health bar
                    glBegin(GL_QUADS);
                    glVertex3f(start.x-1, start.y-1, 0);
                    glVertex3f(end.x+1, start.y-1, 0);
                    glVertex3f(end.x+1, end.y+1, 0);
                    glVertex3f(start.x-1, end.y+1, 0);
                    glEnd();

                    glColor3f(1.0, 0.0, 0.0); //Fills it with red
                    glBegin(GL_QUADS);
                    glVertex3f(start.x, start.y, 0);
                    glVertex3f(end.x, start.y, 0);
                    glVertex3f(end.x, end.y, 0);
                    glVertex3f(start.x, end.y, 0);
                    glEnd();

                    end.x = unit[i].coord[0] - unit[i].width*TILEDIAGONAL/2 + ((float)unit[i].health/(float)unit[i].maxhealth)*unit[i].width*TILEDIAGONAL;
                    glColor3f(0.0, 1.0, 0.0); //Colors part of it green
                    glBegin(GL_QUADS);
                    glVertex3f(start.x, start.y, 0);
                    glVertex3f(end.x, start.y, 0);
                    glVertex3f(end.x, end.y, 0);
                    glVertex3f(start.x, end.y, 0);
                    glEnd();
                }

                /*if(altDown && DEBUG)
                {
                    char txt[100]; sprintf(txt, "id %d owner %d status %d", unit[i].id, unit[i].owner, unit[i].status);
                    float color[3] = {0.5, 0, 0};
                    DrawTextGame(1,unit[i].coord[0] + TILEDIAGONAL/2 - unit[i].width*TILEDIAGONAL, unit[i].coord[1] + TILEDIAGONAL/4 - unit[i].height*TILEDIAGONAL, txt, color);
                }*/
            }

            if(unit[i].status == WORKING) //Increases resource count if the unit is mining
                if(map[unit[i].position[0]][unit[i].position[1]].decoration == 4) //Checks if the current tile contains resources
                            if(unit[i].nextattack < SDL_GetTicks()) //Checks that the units can collect resources at this point
                            {
                                //resources++;
                                unit[i].nextattack = SDL_GetTicks() + unit[i].attackspeed;
                            }
        }
    }
}

void UpdateSeenTiles()
{
    int x, y, i;
    for(x=0; x<MAP_WIDTH; x++)
        for(y=0; y<MAP_HEIGHT; y++)
            map[x][y].seen = 0; //Makes all tiles unseen

    for(i=0; i<=totalUnits; i++) //Makes tiles within sight range seen
        if(unit[i].class != 0)
            if(unit[i].owner == client_id)
                for(x = unit[i].position[0]-unit[i].sight; x <= unit[i].position[0]+unit[i].sight; x++)
                    for(y = unit[i].position[1]-unit[i].sight; y <= unit[i].position[1]+unit[i].sight; y++)
                    {
                        if(x < 0 || x >= MAP_WIDTH || y < 0 || x >= MAP_HEIGHT) continue; //Don't edit tiles that do not exist as it would cause segfaults
                        map[x][y].seen = 1;
                    }
}

void DrawHUD()
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

    float color[3] = {1.0, 1.0, 1.0};
    char txt[100];
    glColor3f(1.0, 1.0, 1.0); //Draws HUD icons
    for(i = 0; i < MAX_TEXTURES_HUD; i++)
    {
        DrawRectangle(1, texture.hud[i],
                      WINDOWWIDTH - (i+1)*(HUD_HEIGHT + HUD_ICON_PADDING), //X position
                      WINDOWHEIGHT - HUD_HEIGHT + HUD_ICON_PADDING, //Y position
                      HUD_HEIGHT - 2 * HUD_ICON_PADDING, //X size
                      HUD_HEIGHT - 2 * HUD_ICON_PADDING); //Y size
        //Draws price tag
        sprintf(txt, "$%i", unit_class[i+1].price);
        DrawTextGame(2,
                     WINDOWWIDTH - (i+1)*(HUD_HEIGHT + HUD_ICON_PADDING) + offset[0]*TILEDIAGONAL,
                     WINDOWHEIGHT - HUD_HEIGHT + HUD_ICON_PADDING + 18 + offset[1]*TILEDIAGONAL/2,
                     txt, color);
    }
    //Draws current resources and population
    sprintf(txt, "Minerals: %i", resources);
    DrawTextGame(2,WINDOWWIDTH/2-30+offset[0]*TILEDIAGONAL, 30+offset[1]*TILEDIAGONAL/2, txt, color);
    sprintf(txt, "Population: %i/%i", population, MAX_POPULATION);
    DrawTextGame(2,WINDOWWIDTH/2-30+offset[0]*TILEDIAGONAL, 60+offset[1]*TILEDIAGONAL/2, txt, color);
}

void LoadTextures()
{
    glEnable(GL_TEXTURE_2D);
    int i, u;
    char temp[100];

    //Load menu textures
    if( (texture.menu[0] = ImageToTexture ( "img/menu/menu.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/menu.png");
    if( (texture.menu[1] = ImageToTexture ( "img/menu/continue1.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/continue1.png");
    if( (texture.menu[2] = ImageToTexture ( "img/menu/help.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/help.png");
    if( (texture.menu[3] = ImageToTexture ( "img/menu/options.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/options.png");
    if( (texture.menu[4] = ImageToTexture ( "img/menu/quit.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/quit.png");
    if( (texture.menu[5] = ImageToTexture ( "img/menu/sure.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/sure.png");
    if( (texture.menu[6] = ImageToTexture ( "img/menu/helpmenu.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/helpmenu.png");
    if( (texture.menu[7] = ImageToTexture ( "img/menu/icon.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/icon.png");
    if( (texture.menu[8] = ImageToTexture ( "img/menu/sound.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/sound.png");
    if( (texture.menu[9] = ImageToTexture ( "img/menu/eyesocket.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/eyesocket.png");
    if( (texture.menu[10] = ImageToTexture ( "img/menu/startmenu.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/startmenu.png");
    if( (texture.menu[11] = ImageToTexture ( "img/menu/optionsmenu.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/optionsmenu.png");
    if( (texture.menu[12] = ImageToTexture ( "img/menu/quitmenu.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/quitmenu.png");
    if( (texture.menu[13] = ImageToTexture ( "img/menu/arrow.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/arrow.png");
    if( (texture.menu[14] = ImageToTexture ( "img/menu/resolutionmenu.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/resolutionmenu.png");
    if( (texture.menu[15] = ImageToTexture ( "img/menu/begin.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/begin.png");
    if( (texture.menu[16] = ImageToTexture ( "img/menu/soundset.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/soundset.png");
    if( (texture.menu[17] = ImageToTexture ( "img/menu/on.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/on.png");
    if( (texture.menu[18] = ImageToTexture ( "img/menu/off.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/off.png");
    if( (texture.menu[19] = ImageToTexture ( "img/menu/eye.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/eye.png");
    if( (texture.menu[20] = ImageToTexture ( "img/menu/blank.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/blank.png");
    if( (texture.menu[21] = ImageToTexture ( "img/menu/grafik.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/grafik.png");
    if( (texture.menu[22] = ImageToTexture ( "img/menu/logo.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/logo.png");
    if( (texture.menu[23] = ImageToTexture ( "img/menu/credits.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/menu/credits.png");

    //Load tile textures
    if( (texture.tile[1] = ImageToTexture ( "img/tiles/grass.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/tiles/grass.png");
    if( (texture.tile[2] = ImageToTexture ( "img/tiles/water.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/tiles/water.png");
    if( (texture.tile[3] = ImageToTexture ( "img/tiles/rock.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/tiles/rock.png");
    if( (texture.tile[4] = ImageToTexture ( "img/tiles/trees.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/tiles/trees.png");
    if( (texture.tile[5] = ImageToTexture ( "img/tiles/sand.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/tiles/sand.png");

    //Load decoration textures
    if( (texture.decoration[1] = ImageToTexture ( "img/decoration/tree1.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/decoration/tree1.png");
    if( (texture.decoration[2] = ImageToTexture ( "img/decoration/tree2.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/decoration/tree2.png");
    if( (texture.decoration[3] = ImageToTexture ( "img/decoration/tree3.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/decoration/tree3.png");
    if( (texture.decoration[4] = ImageToTexture ( "img/decoration/minerals.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/decoration/minerals.png");
    if( (texture.decoration[100] = ImageToTexture ( "img/decoration/star.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/decoration/star.png");

    //Loads HUD textures
    for(i = 0; i < MAX_TEXTURES_HUD; i++)
    {
        sprintf(temp, "img/hud/%i.png", i);
        if( (texture.hud[i] = ImageToTexture ( temp ) ) == NULL )
            fprintf(stderr, "Could not load texture: %s", temp);
    }

    //Loads MapMaker HUD textures
    for(i = 0; i < MAX_TEXTURES_MMHUD; i++)
    {
        sprintf(temp, "img/mmhud/%i.png", i);
        if( (texture.mmhud[i] = ImageToTexture ( temp ) ) == NULL )
            fprintf(stderr, "Could not load texture: %s", temp);
    }

    //Load unit textures
    for(i = 1; i <= UNIT_CLASSES; i++)
        for(u = 0; u < UNIT_DIRECTIONS; u++)
        {
            sprintf(temp, "img/unit/%i/%i.png", i, u);
            if( (texture.unit[i][u] = ImageToTexture ( temp ) ) == NULL )
                fprintf(stderr, "Could not load texture: %s\n", temp);
        }

    //Loads countdown textures
    for(i = 0; i < MAX_TEXTURES_COUNTDOWN; i++)
    {
        sprintf(temp, "img/countdown/%i.png", i);
        if( (texture.countdown[i] = ImageToTexture ( temp ) ) == NULL )
            fprintf(stderr, "Could not load texture: %s\n", temp);
    }

    //Loads endgame status textures
    if( (texture.endgame[0] = ImageToTexture ( "img/endgame/lose.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/endgame/lose.png");
    if( (texture.endgame[1] = ImageToTexture ( "img/endgame/win.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/endgame/win.png");

    //Loads unit status textures
    if( (texture.status[0] = ImageToTexture ( "img/status/idle.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/status/idle.png");
    if( (texture.status[1] = ImageToTexture ( "img/status/moving.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/status/moving.png");
    if( (texture.status[2] = ImageToTexture ( "img/status/following.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/status/following.png");
    if( (texture.status[3] = ImageToTexture ( "img/status/attacking.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/status/attacking.png");
    if( (texture.status[4] = ImageToTexture ( "img/status/working.png" ) ) == NULL )
        fprintf(stderr, "Could not load texture: img/status/working.png");

    glDisable(GL_TEXTURE_2D);
}

void RenderInit()
{
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    if(!(Screen = SDL_SetVideoMode(WINDOWWIDTH, WINDOWHEIGHT, 32,
                                  settings.fullscreen ? (SDL_FULLSCREEN | SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL)
                                  : (SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL))))
    {
        fprintf(stderr,"ERROR: SDL_SetVideoMode");
        exit(1);
    }

    glEnable( GL_TEXTURE_2D );
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, WINDOWWIDTH, WINDOWHEIGHT, 0.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.3);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    LoadTextures();

    if(DEBUG) fprintf(stdout, "RenderInit: Startat OpenGL!\n");
}

void RenderCleanup()
{
    int i;
    glDeleteTextures( MAX_TEXTURES_HUD, (GLuint*)texture.hud ); //Game HUD
    glDeleteTextures( MAX_TEXTURES_MMHUD, (GLuint*)texture.mmhud ); //Mapmaker HUD
    glDeleteTextures( MAX_TEXTURES_TILE, (GLuint*)texture.tile ); //Tiles
    glDeleteTextures( MAX_TEXTURES_MENU, (GLuint*)texture.menu ); //Tiles
    glDeleteTextures( MAX_TEXTURES_DECORATION, (GLuint*)texture.decoration ); //Decoration
    for(i = 1; i <= UNIT_CLASSES; i++) glDeleteTextures( UNIT_DIRECTIONS, (GLuint*)texture.unit[i] ); //Units
    glDeleteTextures( MAX_TEXTURES_COUNTDOWN, (GLuint*)texture.countdown ); //Countdown
    glDeleteTextures( MAX_TEXTURES_ENDGAME, (GLuint*)texture.endgame ); //Endgame
    glDeleteTextures( MAX_TEXTURES_STATUS, (GLuint*)texture.status ); //Status
}

void DrawDetails()
{
    int i, rows = 4, spacing = 20;
    float color[3] = {1.0, 1.0, 1.0};
    char txt[rows][100];
    sprintf(txt[0], "Resolution %ix%i Framerate %i", WINDOWWIDTH, WINDOWHEIGHT, FRAMERATE);
    sprintf(txt[1], "%s @ %s port %i", settings.username, settings.server, settings.port);
    sprintf(txt[2], "Client_id %i totalUnits %i", client_id, totalUnits);
    sprintf(txt[3], "Offset %.2f %.2f", offset[0], offset[1]);
    for(i=0; i<rows; i++) DrawTextGame(1,20+offset[0]*TILEDIAGONAL, 20+i*spacing+offset[1]*TILEDIAGONAL/2, txt[i], color);
}

void DrawPlayerlist()
{
    int i, spacing = 20;
    float color[3] = {1.0, 1.0, 1.0};
    char txt[50];
    for(i=0; i<maxPlayers; i++)
    {
        sprintf(txt, "Client %i - %s", i, connectedClient[i]);
        DrawTextGame(2,WINDOWWIDTH/2-150+offset[0]*TILEDIAGONAL, WINDOWHEIGHT/4+i*spacing+offset[1]*TILEDIAGONAL/2, txt, color);
    }
}

void DrawBox()
{
    int mousePosition[2]; //Checks the current position and updates boxEnd
    SDL_GetMouseState(&mousePosition[0],&mousePosition[1]);
    boxEnd[0] = mousePosition[0]+offset[0]*TILEDIAGONAL;
    boxEnd[1] = mousePosition[1]+offset[1]*TILEDIAGONAL/2;

    glColor3f(settings.boxcolor[0], settings.boxcolor[1], settings.boxcolor[2]); //Sets the box color and draws it

    glBegin(GL_QUADS);
    glVertex3f(boxStart[0], boxStart[1], 0);
    glVertex3f(boxEnd[0], boxStart[1], 0);
    glVertex3f(boxEnd[0], boxStart[1]+settings.boxborder, 0);
    glVertex3f(boxStart[0], boxStart[1]+settings.boxborder, 0);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(boxEnd[0]-settings.boxborder, boxStart[1], 0);
    glVertex3f(boxEnd[0], boxStart[1], 0);
    glVertex3f(boxEnd[0], boxEnd[1], 0);
    glVertex3f(boxEnd[0]-settings.boxborder, boxEnd[1], 0);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(boxEnd[0], boxEnd[1]-settings.boxborder, 0);
    glVertex3f(boxStart[0], boxEnd[1]-settings.boxborder, 0);
    glVertex3f(boxStart[0], boxEnd[1], 0);
    glVertex3f(boxEnd[0], boxEnd[1], 0);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(boxStart[0], boxStart[1], 0);
    glVertex3f(boxStart[0]+settings.boxborder, boxStart[1], 0);
    glVertex3f(boxStart[0]+settings.boxborder, boxEnd[1], 0);
    glVertex3f(boxStart[0], boxEnd[1], 0);
    glEnd();
}

void PerspectiveChange()
{
    if (offsetflag[0] == -1 || offsetflagmouse[0] == -1 || offsetflagscroll[0] == -1) offset[0] -= SCROLLSPEED; //Scrolls the perspective if any arrow keys are pressed or the mouse is close to the window borders
    if (offsetflag[0] == 1 || offsetflagmouse[0] == 1 || offsetflagscroll[0] == 1) offset[0] += SCROLLSPEED;
    if (offsetflag[1] == -1 || offsetflagmouse[1] == -1 || offsetflagscroll[1] == -1) offset[1] -= SCROLLSPEED;
    if (offsetflag[1] == 1 || offsetflagmouse[1] == 1 || offsetflagscroll[1] == 1) offset[1] += SCROLLSPEED;

    //Handles scrolling limits
    if(offset[0] < -WINDOWWIDTH/TILEDIAGONAL/2) offset[0] = -WINDOWWIDTH/TILEDIAGONAL/2;
    if(offset[0] > MAP_WIDTH - WINDOWWIDTH/TILEDIAGONAL/2) offset[0] = MAP_WIDTH - WINDOWWIDTH/TILEDIAGONAL/2;
    if(offset[1] < -MAP_HEIGHT/2 - WINDOWHEIGHT/TILEDIAGONAL) offset[1] = -MAP_HEIGHT/2 - WINDOWHEIGHT/TILEDIAGONAL;
    if(offset[1] > MAP_HEIGHT/2  - WINDOWHEIGHT/TILEDIAGONAL) offset[1] = MAP_HEIGHT/2  - WINDOWHEIGHT/TILEDIAGONAL;

    if(zoomSchedueled != 0) //Zooms according to scollwheel use
    {
        int screenMiddle[2];
        screenMiddle[0] = WINDOWWIDTH/2;
        screenMiddle[1] = WINDOWHEIGHT/2;
        GetIsometricPoint(screenMiddle);
        TILESIZE += ZOOMSPEED*zoomSchedueled;
        TILEDIAGONAL = sqrt(2*pow(TILESIZE,2));
        if(TILESIZE < MIN_ZOOM) TILESIZE = MIN_ZOOM; //Handles maximum and minimum zoom levels
        if(TILESIZE > MAX_ZOOM) TILESIZE = MAX_ZOOM;
        zoomSchedueled = 0;
        offset[0] = (TILEDIAGONAL/2 * screenMiddle[0] + TILEDIAGONAL/2 * screenMiddle[1] - WINDOWWIDTH/2) / TILEDIAGONAL + 1; //Centers camera over the new zoom level
        offset[1] = (TILEDIAGONAL/2 * screenMiddle[0] - TILEDIAGONAL/2 * screenMiddle[1] - WINDOWHEIGHT) / TILEDIAGONAL;
    }
}

void DrawDecorations()
{
    int x, y;
    float isometric[2];

    for(x=0; x<MAP_WIDTH; x++)
    {
        for(y=0; y<MAP_HEIGHT; y++)
        {
            isometric[0] = (x+y+1) * TILEDIAGONAL/2;
            isometric[1] = (x-y-1) * TILEDIAGONAL/4;
            if (  isometric[0] + TILEDIAGONAL - offset[0]*TILEDIAGONAL > 0
               && isometric[0] - TILEDIAGONAL/2 - offset[0]*TILEDIAGONAL < WINDOWWIDTH
               && isometric[1] + TILEDIAGONAL/2 - offset[1]*TILEDIAGONAL/2 > 0
               && isometric[1] - TILEDIAGONAL/4 - offset[1]*TILEDIAGONAL/2 < WINDOWHEIGHT)
            { //Checks if the decoration in question is within the screen boundries and only draws it if it is
                if(map[x][y].seen == 1) glColor3f(1.0, 1.0, 1.0);
                else glColor3f(settings.shader, settings.shader, settings.shader);//Draw decorations fully bright only if seen
                if(map[x][y].decoration != 0 && map[x][y].decoration != 100)
                    DrawRectangle(0, texture.decoration[map[x][y].decoration], isometric[0], isometric[1]  + TILEDIAGONAL/4 - TILEDIAGONAL, TILEDIAGONAL, TILEDIAGONAL);
                if(mapmakerOpen && map[x][y].decoration == 100) DrawRectangle(0, texture.decoration[map[x][y].decoration], isometric[0], isometric[1]  + TILEDIAGONAL/4 - TILEDIAGONAL, TILEDIAGONAL, TILEDIAGONAL);
            }
        }
    }
}

void DrawCountdown()
{
    int value = 5 - (SDL_GetTicks() - countdown) / 1000;
    if(value < 0 || value > 5) countdown = -1;
    else DrawRectangle(1, texture.countdown[value], WINDOWWIDTH/2-WINDOWHEIGHT/4, WINDOWHEIGHT/2-WINDOWHEIGHT/4-HUD_HEIGHT/2, WINDOWHEIGHT/2, WINDOWHEIGHT/2);
}

void DrawEndgame()
{
    DrawRectangle(1, texture.endgame[endgameStatus], WINDOWWIDTH/2-WINDOWHEIGHT/4, WINDOWHEIGHT/2-WINDOWHEIGHT/4-HUD_HEIGHT/2, WINDOWHEIGHT/2, WINDOWHEIGHT/2);
}

void DrawLine(float *from, float *to)
{
    glBegin(GL_LINES);
    glVertex3f(from[0], from[1], 0.0);
    glVertex3f(to[0], to[1], 0.0);
    glEnd();
}

int Render()
{
    int timerStart, waitto;
    RenderInit();
    while(running)
    {
        timerStart = SDL_GetTicks();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(mapmakerOpen)
        {
            DrawMap();
            DrawDecorations();
            MM_DrawHUD();
            PerspectiveChange();
        }
        else if(startMenuOpen) DrawStartMenu();
        else
        {
            DrawMap();
            DrawUnits();
            DrawDecorations();
            if(leftMouseDown) DrawBox();
            DrawHUD();
            DrawChat();
            if(tabDown) DrawPlayerlist();
            if(menuOpen) DrawMenu();
            if(DEBUG) DrawDetails();
            if(countdown != -1) DrawCountdown();
            if(endgameStatus != -1 && showEndgameStatus) DrawEndgame();
            PerspectiveChange();
            if(endgameStatus == -1) UpdateSeenTiles();
        }

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(-(float)offset[0]*TILEDIAGONAL,-(float)offset[1]*TILEDIAGONAL/2,0.0);

        SDL_GL_SwapBuffers();

        waitto = timerStart + 1000/FRAMERATE;
        while(SDL_GetTicks() < waitto);
    }

    RenderCleanup(); //Cleans up textures at exit
    return 0;
}
