/*
 * File:   event.c
 * Author: Projektgrupp 4
 * Created by: Tobias Hultgren
 * Revisioned by: David Wikmans
 *
 * Created on April 2, 2011
 */

#include "header.h"
#include "net.h"
#include "sound.h"
#include "menu.h"
#include "mapmaker.h"

void Event(SDL_Event* event)
{
	int mousePosition[2], selected_unit;
	static int keytaps = 0; // Något slags fulhax
    if(drawconsole){
        char legal[] = "abcdefghijklmnopqrstuvwxyzåäö1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZÅÄÖ,.-;:_*^~!¤%&/()=?+\\<>|øæ ";
        if(event->type == SDL_KEYUP){
            if(event->key.keysym.sym == SDLK_RETURN){
                if(strlen(console)>0){
                    char buff[PACKETSIZE];
                    strncpy(buff, console ,PACKETSIZE);
                    cmdsend(1,0,client_id,console,NULL);
                }
                drawconsole = false;
                int i;
                for(i=0; i<MESSAGESIZE; i++) console[i] = '\0';
                keytaps=0;
                if(DEBUG) fprintf(stderr,"Chat toggle = FALSE\n");
            }
        } else if(event->type == SDL_KEYDOWN) {
            if(event->key.keysym.sym == SDLK_BACKSPACE){
                int len = strlen(console);
                if(keytaps>0) keytaps--;
                console[len-1 > 0 ? len-1 : 0] = '\0';
            } else {
                int i;
                if(strlen(console)+1 < MESSAGESIZE){
                    for(i=0; i<strlen(legal); i++){
                        if(legal[i] == event->key.keysym.unicode){
                            console[keytaps] = event->key.keysym.unicode;
                            keytaps++;
                            break;
                        }
                    }
                }
            }
        }
    }
    else if(mapmakerOpen) mmEvent(event);
    else if(menuOpen) menuEvent(event);
    else if(startMenuOpen) startMenuEvent(event);
    else //In-game
    {
        switch (event->type)
        {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                switch (event->key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    case SDLK_q:
                        running = false;
                        break;
                    case SDLK_F10:
                        offsetflag[0] = 0;
                        offsetflag[1] = 0;
                        altDown = false;
                        tabDown = false;
                        menuOpen = true;
                        break;
                    case SDLK_CAPSLOCK:
                        capsActive = true;
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
                    case SDLK_m:
                        PlayGameSound(MUSIC);
                        break;
                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                        shiftDown = true;
                        if(DEBUG) fprintf(stderr,"Shift Down\n");
                        break;
                    case SDLK_LALT:
                        altDown = true;
                        if(DEBUG) fprintf(stderr,"Alt Down\n");
                        break;
                    case SDLK_LCTRL:
                    case SDLK_RCTRL:
                        ctrlDown = true;
                        if(DEBUG) fprintf(stderr,"Ctrl Down\n");
                        break;
                    case SDLK_TAB:
                        cmdsend(80,client_id,0," "," ");
                        tabDown = true;
                        if(DEBUG) fprintf(stderr,"TAB Down\n");
                        break;
                    case SDLK_0: //Handles control groups
                    case SDLK_1:
                    case SDLK_2:
                    case SDLK_3:
                    case SDLK_4:
                    case SDLK_5:
                    case SDLK_6:
                    case SDLK_7:
                    case SDLK_8:
                    case SDLK_9:
                        if(ctrlDown)
                        {
                            SetControlGroup(event->key.keysym.sym-48);
                            if(DEBUG)fprintf(stderr,"Setting control group: %i\n",event->key.keysym.sym-48);
                        }
                        else
                        {
                            GetControlGroup(event->key.keysym.sym-48);
                            if(DEBUG)fprintf(stderr,"Getting control group: %i\n",event->key.keysym.sym-48);
                        }
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event->key.keysym.sym)
                {
                    case SDLK_RETURN:
                        drawconsole = true;
                        fprintf(stderr, "Chat toggle = TRUE\n");
                        break;
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
                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                        shiftDown = false;
                        if(DEBUG) fprintf(stderr,"Shift Up\n");
                        break;
                    case SDLK_LALT:
                        altDown = false;
                        if(DEBUG) fprintf(stderr,"Alt Up\n");
                        break;
                    case SDLK_LCTRL:
                    case SDLK_RCTRL:
                        ctrlDown = false;
                        if(DEBUG) fprintf(stderr,"Ctrl Up\n");
                        break;
                    case SDLK_TAB:
                        tabDown = false;
                        if(DEBUG) fprintf(stderr,"TAB Up\n");
                        break;
                    case SDLK_CAPSLOCK:
                        capsActive = false;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&mousePosition[0],&mousePosition[1]);
                if(   mousePosition[0] < 5 || mousePosition[0] > WINDOWWIDTH-5 //Checks if mouse is close to the screen borders
                   || mousePosition[1] < 5 || mousePosition[1] > WINDOWHEIGHT-5)
                {
                    if(mousePosition[0] < 5) offsetflagmouse[0] = -1; //Sets offset flags according to mouse position
                    if(mousePosition[0] > WINDOWWIDTH-5) offsetflagmouse[0] = 1;
                    if(mousePosition[1] < 5) offsetflagmouse[1] = -1;
                    if(mousePosition[1] > WINDOWHEIGHT-5) offsetflagmouse[1] = 1;
                }
                else //Sets offset flags to 0 if mouse is not close to the borders
                {
                    offsetflagmouse[0] = 0;
                    offsetflagmouse[1] = 0;
                }
                if(scrollDown)
                {
                    SDL_GetMouseState(&mousePosition[0],&mousePosition[1]);
                    if(mousePosition[0] > scrollDownStart[0] + 10) offsetflagscroll[0] = 1;
                    if(mousePosition[0] < scrollDownStart[0] - 10) offsetflagscroll[0] = -1;
                    if(mousePosition[1] > scrollDownStart[1] + 10) offsetflagscroll[1] = 1;
                    if(mousePosition[1] < scrollDownStart[1] - 10) offsetflagscroll[1] = -1;
                }

                if(leftMouseDown)
                {
                    boxEnd[0] = mousePosition[0]+offset[0]*TILEDIAGONAL;
                    boxEnd[1] = mousePosition[1]+offset[1]*TILEDIAGONAL/2;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                {
                    SDL_GetMouseState(&mousePosition[0],&mousePosition[1]);
                    switch (event->button.button)
                    {
                        case SDL_BUTTON_LEFT:
                            if(showEndgameStatus) showEndgameStatus = false;
                            lastSelectedGroup = -1;
                            if(mousePosition[1] < WINDOWHEIGHT-HUD_HEIGHT) //If the mouse is within the game area
                            {
                                SetSelected(mousePosition);
                                SDL_GetMouseState(&mousePosition[0],&mousePosition[1]);
                                boxStart[0] = mousePosition[0]+offset[0]*TILEDIAGONAL;
                                boxStart[1] = mousePosition[1]+offset[1]*TILEDIAGONAL/2;
                                boxEnd[0] = boxStart[0]; //Prevents graphical errors
                                boxEnd[1] = boxStart[1];
                                leftMouseDown = true;
                            }
                            else
                            {
                                hudClick = true;
                                ClickHUDButton(mousePosition);
                            }
                            break;
                        case SDL_BUTTON_RIGHT:
                            if(mousePosition[1] < WINDOWHEIGHT-HUD_HEIGHT) { //If the mouse is within the game area
                                //fprintf(stdout,"RIGHTCLICK\n");
                                selected_unit = SetTarget(mousePosition);
                                SDL_GetMouseState(&mousePosition[0],&mousePosition[1]); //New mouse coordinates as SetTarget manipulates them
                                SendUnits(mousePosition,selected_unit);
/*                                if ( selected_unit == -1 )//Tries to set target
                                {
                                    if ( SetDestination(mousePosition) == true  ) //Only set destination if target set failed
                                    {
                                        if(DEBUG) fprintf(stdout,"Sending selected unit(s) to %i,%i\n",mousePosition[0],mousePosition[1]);
                                    }
                                }
                                else if (unit[selected_unit].owner == client_id) {
                                fprintf(stdout,"Following unit %i\n",selected_unit);
                                }
                                else if (unit[selected_unit].owner != client_id) {
                                fprintf(stdout,"Attacking unit %i\n",selected_unit);
                                }*/
                            }
                            break;
                        case SDL_BUTTON_WHEELUP:
                            zoomSchedueled += 1;
                            break;
                        case SDL_BUTTON_WHEELDOWN:
                            zoomSchedueled -= 1;
                            break;
                        case SDL_BUTTON_MIDDLE:
                            scrollDown = true;
                            SDL_GetMouseState(&scrollDownStart[0],&scrollDownStart[1]);
                            if(DEBUG) fprintf(stderr,"Middle mouse button Down\n");
                            break;
                    }
                }
                break;
            case SDL_MOUSEBUTTONUP:
                {
                    switch (event->button.button)
                    {
                        case SDL_BUTTON_LEFT:
                            if(hudClick)
                            {
                                hudClick = false;
                                break;
                            }
                            leftMouseDown = false;
                            int temp;
                            SDL_GetMouseState(&mousePosition[0],&mousePosition[1]);
                            boxEnd[0] = mousePosition[0]+offset[0]*TILEDIAGONAL;
                            boxEnd[1] = mousePosition[1]+offset[1]*TILEDIAGONAL/2;
                            boxDiff[0] = boxStart[0] - boxEnd[0];
                            boxDiff[1] = boxStart[1] - boxEnd[1];
                            if(boxDiff[0] < 0)
                            {
                                boxDiff[0] *= -1;
                            }
                            else
                            {
                                temp = boxStart[0];
                                boxStart[0] = boxEnd[0];
                                boxEnd[0] = temp;
                            }
                            if(boxDiff[1] < 0)
                            {
                                boxDiff[1] *= -1;
                            }
                            else
                            {
                                temp = boxStart[1];
                                boxStart[1] = boxEnd[1];
                                boxEnd[1] = temp;
                            }
                            if(boxDiff[0] > BOX_LIMIT || boxDiff[1] > BOX_LIMIT) SetSelectedGroup(); //Only use the SetSelectedGroup function if the box is large enough
                            break;
                        case SDL_BUTTON_MIDDLE:
                            scrollDown = false;
                            offsetflagscroll[0] = 0;
                            offsetflagscroll[1] = 0;
                            if(DEBUG) fprintf(stderr,"Middle mouse button Up\n");
                            break;
                    }
                }
                break;
            }
        }
}
