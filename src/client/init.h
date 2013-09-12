/*
 * File:   init.h
 * Author: Projektgrupp 4
 * Created by: Tobias Hultgren
 * Revisioned by:
 *
 * Created on April 19, 2011
 */

//Provides starting values to global variables
void InitVariables(); //Tobias

//Creates a new settings file
void NewSettings(); //Tobias

//Loads game settings from the settings.cfg file
void LoadSettings(); //Tobias

//Loads class information from class.dat and stores it in the unit_class array
void LoadClasses(); //Tobias

//Loads map to the tile matrix
void LoadMap(); //Tobias

//Initiates SDL
void InitSDL(); //Tobias

//Initiates audio
void InitAudio(); //Emil

//Reset map and units
void ResetWorld(); //David
