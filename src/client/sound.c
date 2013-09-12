/*
 * File:   sound.c
 * Author: Projektgrupp 4
 * Created by: Emil Lundmark
 * Revisioned by:
 *
 * Created on May 2, 2011
 */

#include "header.h"
#include "sound.h"

/*Function is called with enums sounds depending on what sound is needed*/
void PlayGameSound(enum sounds id)
{
    switch( id )
    {
        case ATTACK:
            randomsound = rand()%3;
            switch( randomsound )
            {
                case 0:
                    if ( Mix_PlayChannel( -1, attack_sound, 0 ) == -1 )
                        fprintf(stderr,"Error in function: PlayGameSound > Mix_PlayChannel(attack_sound): %s\n",Mix_GetError());
                    break;
                case 1:
                    if ( Mix_PlayChannel( -1, attack_sound2, 0 ) == -1 )
                        fprintf(stderr,"Error in function: PlayGameSound > Mix_PlayChannel(attack_sound2): %s\n",Mix_GetError());
                    break;
                case 2:
                    if ( Mix_PlayChannel( -1, attack_sound3, 0 ) == -1 )
                        fprintf(stderr,"Error in function: PlayGameSound > Mix_PlayChannel(attack_sound3): %s\n",Mix_GetError());
                    break;
            }
            break;

        case FOLLOW:
            if ( Mix_PlayChannel( -1, follow_sound, 0 ) == -1 )
                fprintf(stderr,"Error in function: PlayGameSound > Mix_PlayChannel(follow_sound): %s\n",Mix_GetError());
            break;

        case LOST:
            if ( Mix_PlayChannel( -1, lose_sound, 0 ) == -1 )
                fprintf(stderr,"Error in function: PlayGameSound > Mix_PlayChannel(unitlost_sound_sound): %s\n",Mix_GetError());
            break;

        case WON:
            if ( Mix_PlayChannel( -1, win_sound, 0 ) == -1 )
                fprintf(stderr,"Error in function: PlayGameSound > Mix_PlayChannel(won_sound): %s\n",Mix_GetError());
            break;

        case UNDER_ATTACK:
            if(lastAttacked + 5000 < SDL_GetTicks())
            {
                if ( Mix_PlayChannel( -1, underattack_sound, 0 ) == -1 )
                    fprintf(stderr,"Error in function: PlayGameSound > Mix_PlayChannel(underattack_sound): %s\n",Mix_GetError());
                lastAttacked = SDL_GetTicks();
            }
            break;

        case WORK:
            if ( Mix_PlayChannel( -1, work_sound, 0 ) == -1 )
                fprintf(stderr,"Error in function: PlayGameSound > Mix_PlayChannel(work_sound): %s\n",Mix_GetError());
            break;

        case UNIT_LOST:
            if ( Mix_PlayChannel( -1, unitlost_sound, 0 ) == -1 )
                fprintf(stderr,"Error in function: PlayGameSound > Mix_PlayChannel(unitlost_sound): %s\n",Mix_GetError());
            break;

        case MOVE:
            randomsound = rand()%3;
            switch( randomsound )
            {
                case 0:
                    if ( Mix_PlayChannel( -1, move_sound1, 0 ) == -1 )
                        fprintf(stderr,"Error in function: PlayGameSound > Mix_PlayChannel(move_sound1): %s\n",Mix_GetError());
                    break;

                case 1:
                    if ( Mix_PlayChannel( -1, move_sound2, 0 ) == -1 )
                        fprintf(stderr,"Error in function: PlayGameSound > Mix_PlayChannel(move_sound2: %s\n",Mix_GetError());
                    break;

                case 2:
                    if ( Mix_PlayChannel( -1, move_sound3, 0 ) == -1 )
                        fprintf(stderr,"Error in function: PlayGameSound > Mix_PlayChannel(move_sound3): %s\n",Mix_GetError());
                    break;

                default:
                    break;
            }
            break;

        case MUSIC:
            if( Mix_PlayingMusic() == 0 )
            {
                if( Mix_PlayMusic( music_sound, -1 ) == -1 )
                    fprintf(stderr,"Error in function: PlayGameSound > Mix_PlayMusic(music_sound): %s\n",Mix_GetError());
            }
            else
            {
                if( Mix_PausedMusic() == 1 ) {Mix_ResumeMusic();}
                else {Mix_PauseMusic();}
            }
            break;

        case CANNOT:
            if ( Mix_PlayChannel( -1, cantwalk_sound, 0 ) == -1 )
                fprintf(stderr,"Error in function: PlayGameSound > Mix_PlayChannel(cantwalk_sound): %s\n",Mix_GetError());
            break;

        case MENU:
            if ( Mix_PlayChannel( -1, menu_sound, 0 ) == -1 )
                fprintf(stderr,"Error in function: PlayGameSound > Mix_PlayChannel(menu_sound): %s\n",Mix_GetError());
            break;

        default:
            break;
    }
}
/*Function is called with enum sounds, and changes game volume*/
void ChangeGameVolume(enum sounds id)
{
    switch(id)
    {
        case INCREASE:
            if(gameVolume < 128)
            {
                gameVolume+=16;
                Mix_Volume (-1,gameVolume );
            }
            break;

        case DECREASE:
            if(gameVolume > 0)
            {
                gameVolume-=16;
                Mix_Volume (-1,gameVolume );
            }
            break;

        default:
            break;
    }
}
/*Function is called with enum sounds, and changes music volume*/
void ChangeMusicVolume(enum sounds id)
{
    switch(id)
    {
        case INCREASE:
            if(musicVolume < 128)
            {
                musicVolume+=16;
                Mix_VolumeMusic (musicVolume );
            }
            break;

        case DECREASE:
            if(musicVolume > 0)
            {
                musicVolume-=16;
                Mix_VolumeMusic (musicVolume );
            }
            break;
        default:
            break;
    }
}
/*This function is called from the menu, to change menu menu music to ingame music*/
void ChangeMusic ()
{
    Mix_HaltMusic();
    music_sound = Mix_LoadMUS( "sound/drifter.ogg" );
    if ( music_sound == NULL )
        fprintf(stderr,"Error in function: InitAudio() > Mix_LoadMUS(sound/drifter.ogg): %s\n",Mix_GetError());
}
