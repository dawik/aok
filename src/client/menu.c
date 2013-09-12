/*
 * File:   menu.c
 * Author: Projektgrupp 4
 * Created by: Adam Frankenberg
 * Revisioned by:
 *
 * Created on April 2, 2011
 */

#include "header.h"
#include "menu.h"
#include "render.h"
int start = 5;

//events in start menu
void startMenuEvent(SDL_Event* event)
{
    if(Mix_PlayingMusic() == 0) PlayGameSound ( MUSIC );
    switch (event->type)
    {
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym)
            {
                case SDLK_UP:
                //if a key is down it plays a sound, and the arrow moves on the display to a plays it can move
                    PlayGameSound( MENU );
                    if(menu==4 && arrow == 4)arrow=0;
                    if(menu==6 && arrow == 2)arrow=0;
                    arrow--;
                    break;
                case SDLK_DOWN:
                    PlayGameSound( MENU );
                    if(menu==4 && arrow == 4)arrow=3;
                    if(menu==6 && arrow == 4)arrow=1;
                    arrow++;
                    break;
                case SDLK_LEFT:
                    PlayGameSound( MENU );
                    if(menu==4 && arrow == 4)arrow=0;
                    if(menu==6 && arrow == 2)arrow=0;
                    arrow--;
                    break;
                case SDLK_RIGHT:
                    PlayGameSound( MENU );
                    if(menu==4 && arrow == 4)arrow=3;
                    if(menu==6 && arrow == 4)arrow=1;
                    arrow++;
                    break;
               default:
                    break;

            }
        break;
        case SDL_KEYUP:
            switch (event->key.keysym.sym)
            {
                case SDLK_RETURN:
                    PlayGameSound( MENU );
                    switch(menu)
                    //if enter is pressed the arrow tells the flag were it is
                    {
                        case 0: start = arrow; break;
                        //checks if fullscreen is off or on then swaps
                        case 1: options = arrow;
                                if(fullscreen == 17 && arrow == 0) {fullscreen=18;settings.fullscreen = 0; RenderInit();} else if(arrow == 0) {fullscreen = 17;settings.fullscreen = 1;RenderInit();}break;
                        case 2: resolution = arrow;break;
                        case 3: quit = arrow;break;
                        case 4: credits = arrow;break;
                        case 5: soundset = arrow;break;
                        case 6: grafik = arrow;
                                if(textur == 17 && arrow == 2) {textur=18;settings.textures = 0;} else if(arrow == 2) {textur = 17;settings.textures = 1;}
                                if(inter == 17 && arrow == 3) {inter=18;settings.interpolation = 0;} else if(arrow == 3) {inter = 17;settings.interpolation = 1;}break;

                    }
                    break;

                default:
                break;

            }
        int mousePosition[2];
            case SDL_MOUSEMOTION:
            //follow the mouse
            SDL_GetMouseState(&mousePosition[0],&mousePosition[1]);
            x = mousePosition[0]-WINDOWHEIGHT/9;
            y = mousePosition[1]-WINDOWHEIGHT/15;




    }
}
//in-gamemenu events
void menuEvent(SDL_Event* event)
{
    int mousePosition[2];
    switch (event->type)
    {
        case SDL_MOUSEMOTION:
        SDL_GetMouseState(&mousePosition[0],&mousePosition[1]);
            switch (event->button.button)
            {
                //contol the sound in ingame menu
                //checks  were the button is
                case SDL_BUTTON_LEFT:
                    if(optionsmenu==true)
                    {
                        settings.volume = mousePosition[0];
                        if(settings.volume>(WINDOWWIDTH/2+WINDOWHEIGHT/1.5/3))settings.volume=WINDOWWIDTH/2+WINDOWHEIGHT/1.5/3;
                        if(settings.volume<(WINDOWWIDTH/2-WINDOWHEIGHT/1.5/3))settings.volume=WINDOWWIDTH/2-WINDOWHEIGHT/1.5/3;
                        if(settings.volume>volume1+WINDOWHEIGHT/25){ChangeMusicVolume(INCREASE);volume1 = settings.volume;}
                        if(settings.volume<volume1-WINDOWHEIGHT/25){ChangeMusicVolume(DECREASE);volume1 = settings.volume;}

                    }
                break;
            }

            break;

        case SDL_MOUSEBUTTONDOWN:
        SDL_GetMouseState(&mousePosition[0],&mousePosition[1]);
        //if something is pressed in the in-gamemenu
            switch (event->button.button)
            {
                case SDL_BUTTON_LEFT:
                    if(mousePosition[0]>WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2 && mousePosition[0]<WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2+WINDOWHEIGHT/2.3+offset[0] && mousePosition[1]>WINDOWHEIGHT/6.5 && mousePosition[1]<WINDOWHEIGHT/6.5+WINDOWHEIGHT/8){menuOpen = false;optionsmenu = false; test = false;helpmenu = false;}
                    if(helpmenu==false && optionsmenu==false && mousePosition[0]>WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2 && mousePosition[0]<WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2+WINDOWHEIGHT/2.3 && mousePosition[1]>WINDOWHEIGHT/1.45 && mousePosition[1]<WINDOWHEIGHT/1.45+WINDOWHEIGHT/8)test = true;
                    if(test==false && optionsmenu==false && mousePosition[0]>WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2 && mousePosition[0]<WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2+WINDOWHEIGHT/2.3 && mousePosition[1]>WINDOWHEIGHT/3.0 && mousePosition[1]<WINDOWHEIGHT/3.0+WINDOWHEIGHT/8)helpmenu = true;
                    if(helpmenu==false && test==false && mousePosition[0]>WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2 && mousePosition[0]<WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2+WINDOWHEIGHT/2.3 && mousePosition[1]>WINDOWHEIGHT/1.95 && mousePosition[1]<WINDOWHEIGHT/1.95+WINDOWHEIGHT/8)optionsmenu = true;
                    if(test==true && helpmenu==false && optionsmenu==false)
                    {
                        if(mousePosition[0]>WINDOWWIDTH/2 && mousePosition[0]<WINDOWWIDTH/2-WINDOWHEIGHT/5.3/2+WINDOWHEIGHT/4.2 && mousePosition[1]>WINDOWHEIGHT/2.1 && mousePosition[1]<WINDOWHEIGHT/2.1+WINDOWHEIGHT/12)test = false;
                        if(mousePosition[0]>WINDOWWIDTH/2-WINDOWHEIGHT/3.2/2 && mousePosition[0]<WINDOWWIDTH/2-WINDOWHEIGHT/3.8+WINDOWHEIGHT/4.2 && mousePosition[1]>WINDOWHEIGHT/2.1 && mousePosition[1]<WINDOWHEIGHT/2.1+WINDOWHEIGHT/12)running = false;
                    }
                    break;
            }
            break;
    }
}

void DrawStartMenu()
//start menu
{
    DrawRectangle(1,texture.menu[20],0,0,WINDOWWIDTH,WINDOWHEIGHT);
    DrawRectangle(1,texture.menu[19],x,y,WINDOWWIDTH/7,WINDOWHEIGHT/7);
    DrawRectangle(1,texture.menu[9],0,0,WINDOWWIDTH,WINDOWHEIGHT);
    DrawRectangle(1,texture.menu[22],WINDOWWIDTH/2-WINDOWHEIGHT/1.55/2,WINDOWHEIGHT/5,WINDOWHEIGHT/1.5,WINDOWHEIGHT/10);

    //drawing the arrow
    switch(arrow)
    {
        case -1: DrawRectangle(1,texture.menu[13],WINDOWWIDTH/2-WINDOWHEIGHT/2.9/2,WINDOWHEIGHT/1.65,WINDOWHEIGHT/30,WINDOWHEIGHT/30);arrow = 4; break;
        case 0: DrawRectangle(1,texture.menu[13],WINDOWWIDTH/2-WINDOWHEIGHT/2.9/2,WINDOWHEIGHT/2.7,WINDOWHEIGHT/30,WINDOWHEIGHT/30); break;
        case 1: DrawRectangle(1,texture.menu[13],WINDOWWIDTH/2-WINDOWHEIGHT/2.9/2,WINDOWHEIGHT/2.3,WINDOWHEIGHT/30,WINDOWHEIGHT/30); break;
        case 2: DrawRectangle(1,texture.menu[13],WINDOWWIDTH/2-WINDOWHEIGHT/2.9/2,WINDOWHEIGHT/2.04,WINDOWHEIGHT/30,WINDOWHEIGHT/30); break;
        case 3: DrawRectangle(1,texture.menu[13],WINDOWWIDTH/2-WINDOWHEIGHT/2.9/2,WINDOWHEIGHT/1.83,WINDOWHEIGHT/30,WINDOWHEIGHT/30); break;
        case 4: DrawRectangle(1,texture.menu[13],WINDOWWIDTH/2-WINDOWHEIGHT/2.9/2,WINDOWHEIGHT/1.65,WINDOWHEIGHT/30,WINDOWHEIGHT/30); break;
        case 5: DrawRectangle(1,texture.menu[13],WINDOWWIDTH/2-WINDOWHEIGHT/2.9/2,WINDOWHEIGHT/2.7,WINDOWHEIGHT/30,WINDOWHEIGHT/30);arrow = 0; break;
        case 6: DrawRectangle(1,texture.menu[13],WINDOWWIDTH/2+WINDOWHEIGHT/300.0/2,WINDOWHEIGHT/1.9,WINDOWHEIGHT/30,WINDOWHEIGHT/30);arrow = 8; break;
        case 7: DrawRectangle(1,texture.menu[13],WINDOWWIDTH/2+WINDOWHEIGHT/300.0/2,WINDOWHEIGHT/1.9,WINDOWHEIGHT/30,WINDOWHEIGHT/30); break;
        case 8: DrawRectangle(1,texture.menu[13],WINDOWWIDTH/2-WINDOWHEIGHT/3.0/2,WINDOWHEIGHT/1.9,WINDOWHEIGHT/30,WINDOWHEIGHT/30); break;
        case 9: DrawRectangle(1,texture.menu[13],WINDOWWIDTH/2-WINDOWHEIGHT/3.0/2,WINDOWHEIGHT/1.9,WINDOWHEIGHT/30,WINDOWHEIGHT/30);arrow = 7; break;

    }
    //checks if interpolation is on or off
    if(settings.interpolation == 0) inter = 18;
    else
        inter = 17;
    if(settings.textures == 0) textur = 18;
    else
        textur = 17;
    if(settings.fullscreen == 0) fullscreen = 18;
    else
        fullscreen = 17;

//drawing the actuell texture with help off the flags
        switch(menu)
        {
            case 0:
            DrawRectangle(1,texture.menu[10],WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2,WINDOWHEIGHT/3.0,WINDOWHEIGHT/2.3,WINDOWHEIGHT/3);
                switch(start)
                {
                    //game starts
                    case 0: startMenuOpen = false;
                            settings.volume = WINDOWWIDTH/2 + settings.volume;
                            if(settings.volume>(WINDOWWIDTH/2+WINDOWHEIGHT/1.5/3))settings.volume=WINDOWWIDTH/2+WINDOWHEIGHT/1.5/3;
                            if(settings.volume<(WINDOWWIDTH/2-WINDOWHEIGHT/1.5/3))settings.volume=WINDOWWIDTH/2-WINDOWHEIGHT/1.5/3;
                            Mix_HaltMusic();
                            ChangeMusic();
                            break;
                    //open mapmaker
                    //start is 5 to avoid geting traped in a loop when trying to come back
                    case 1: start = 5;
                            arrow = 0;
                            mapmakerOpen = true;
                            MapMaker();
                            break;
                    case 2: menu = 1; arrow = 0; break;
                    case 3: menu = 4; arrow = 4;credits = 1;  break;
                    case 4: menu = 3; arrow=7; quit = 2;break;
                }
            break;

            case 1:
                DrawRectangle(1,texture.menu[11],WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2,WINDOWHEIGHT/3.0,WINDOWHEIGHT/2.3,WINDOWHEIGHT/3);
                DrawRectangle(1,texture.menu[fullscreen],WINDOWWIDTH/2+WINDOWHEIGHT/2.7/2,WINDOWHEIGHT/3.2,WINDOWHEIGHT/6,WINDOWHEIGHT/6);
                switch(options)
                    {
                        case 0:break;
                        case 1:menu = 6; arrow = 2; options = 0; grafik = 6; break;
                        case 2:menu = 5; arrow = 0; options = 0; soundset = 6; break;
                        case 3:menu = 2; arrow = 0; options = 0; resolution = 6; break;
                        case 4:menu = 0; arrow = 0; options = 0;start = 5; break;
                    }
            break;
            case 2:
                DrawRectangle(1,texture.menu[14],WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2,WINDOWHEIGHT/3.0,WINDOWHEIGHT/2.3,WINDOWHEIGHT/3);
                switch(resolution)
                    {
                        case 0: WINDOWWIDTH = 640; WINDOWHEIGHT = 480;resolution = 6;RenderInit(); break;
                        case 1: WINDOWWIDTH = 800; WINDOWHEIGHT = 600;resolution = 6;RenderInit(); break;
                        case 2: WINDOWWIDTH = 1024; WINDOWHEIGHT = 768;resolution = 6;RenderInit(); break;
                        case 3: WINDOWWIDTH = 1280; WINDOWHEIGHT = 1024;resolution = 6;RenderInit(); break;
                        case 4: menu = 1; arrow = 0; resolution = 0; break;
                    }
            break;
            case 3:
                DrawRectangle(1,texture.menu[12],WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2,WINDOWHEIGHT/3.0,WINDOWHEIGHT/2.3,WINDOWHEIGHT/3);
                    switch(quit)
                        {
                            case 7:arrow = 0; quit = 0; menu = 0; start = 5; break;
                            case 8:running = 0; startMenuOpen = 0;break;
                        }

            break;
            case 4:
                DrawRectangle(1,texture.menu[23],WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2,WINDOWHEIGHT/3.0,WINDOWHEIGHT/2.3,WINDOWHEIGHT/3);
                switch(credits)
                    {
                        case 4: menu = 0; arrow = 0;credits = 0; start = 5; break;
                    }
            break;
            case 5:
                DrawRectangle(1,texture.menu[16],WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2,WINDOWHEIGHT/3.0,WINDOWHEIGHT/2.3,WINDOWHEIGHT/3);
                switch(soundset)
                    {
                        case 0: ChangeMusicVolume(INCREASE);soundset = 6; settings.volume = settings.volume + WINDOWHEIGHT/25; break;
                        case 1: ChangeMusicVolume(DECREASE);soundset = 6;settings.volume = settings.volume - WINDOWHEIGHT/25; break;
                        case 2: ChangeGameVolume(INCREASE);soundset = 6; PlayGameSound(ATTACK);break;
                        case 3: ChangeGameVolume(DECREASE);soundset = 6; PlayGameSound(ATTACK);break;
                        case 4: menu = 0; arrow = 0; soundset = 0; break;
                    }


        break;
        case 6:
            DrawRectangle(1,texture.menu[21],WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2,WINDOWHEIGHT/3.0,WINDOWHEIGHT/2.3,WINDOWHEIGHT/3);
            DrawRectangle(1,texture.menu[textur],WINDOWWIDTH/2+WINDOWHEIGHT/2.7/2,WINDOWHEIGHT/2.35,WINDOWHEIGHT/6,WINDOWHEIGHT/6);
            DrawRectangle(1,texture.menu[inter],WINDOWWIDTH/2+WINDOWHEIGHT/2.7/2,WINDOWHEIGHT/2.05,WINDOWHEIGHT/6,WINDOWHEIGHT/6);
                switch(grafik)
                {
                    case 0:break;
                    case 4:grafik = 0;menu = 1; arrow = 0; break;
                }
        break;

        }
}

void DrawMenu()

{
    //open in-gamemenu
    glColor3f(1.0, 1.0, 1.0);
    DrawRectangle(1,texture.menu[0],WINDOWWIDTH/2-WINDOWHEIGHT/1.5/2,WINDOWHEIGHT/12,WINDOWHEIGHT/1.5,WINDOWHEIGHT/1.2);
    DrawRectangle(1,texture.menu[1],WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2,WINDOWHEIGHT/10,WINDOWHEIGHT/2.3,WINDOWHEIGHT/4);
    if(test==false && helpmenu==false && optionsmenu==false)
    {
        DrawRectangle(1,texture.menu[2],WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2,WINDOWHEIGHT/3.6,WINDOWHEIGHT/2.3,WINDOWHEIGHT/4);
        DrawRectangle(1,texture.menu[3],WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2,WINDOWHEIGHT/2.2,WINDOWHEIGHT/2.3,WINDOWHEIGHT/4);
        DrawRectangle(1,texture.menu[4],WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2,WINDOWHEIGHT/1.6,WINDOWHEIGHT/2.3,WINDOWHEIGHT/4);
    }
    //when quit is pressed
    if(test==true && helpmenu==false && optionsmenu==false)
    {
        DrawRectangle(1,texture.menu[5],WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2,WINDOWHEIGHT/3.6,WINDOWHEIGHT/1.8,WINDOWHEIGHT/2.8);
    }
    //if sound is pressed
    if(optionsmenu==true && helpmenu==false && test==false)
    {
        DrawRectangle(1,texture.menu[1],WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2,WINDOWHEIGHT/10,WINDOWHEIGHT/2.3,WINDOWHEIGHT/4);
        DrawRectangle(1,texture.menu[8],WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2,WINDOWHEIGHT/2.6,WINDOWHEIGHT/2.2,WINDOWHEIGHT/4);
        DrawRectangle(1,texture.menu[7],settings.volume,WINDOWHEIGHT/2.0,WINDOWHEIGHT/30,WINDOWHEIGHT/30);
    }
    //if help is pressed
    if(helpmenu==true && optionsmenu==false && test==false)
    {
       DrawRectangle(1,texture.menu[6],WINDOWWIDTH/2-WINDOWHEIGHT/2.3/2,WINDOWHEIGHT/2.6,WINDOWHEIGHT/2.3,WINDOWHEIGHT/2.5);
    }
}
