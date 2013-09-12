/*
 * File:   cleanup.c
 * Author: Projektgrupp 4
 * Created by: Tobias Hultgren
 * Revisioned by:
 *
 * Created on April 2, 2011
 */

#include "header.h"
#include "sound.h"

void Cleanup()
{
    Mix_CloseAudio();
    SDL_FreeSurface(Screen);
    SDL_Quit();
}
