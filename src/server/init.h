/*
 * File:   init.h
 * Author: Projektgrupp 4
 * Created by: Tobias Hultgren
 * Revisioned by:
 *
 * Created on April 19, 2011
 */

//Provides starting values to global variables
void InitVariables(); //David

//Loads game settings from the settings.cfg file
void LoadSettings(); //Tobias & Christoffer

//Loads class information from class.dat and stores it in the unit_class array
void LoadClasses(); //Tobias

//Loads map to the tile matrix
int LoadMap(); //Tobias

//Reset map and units
void ResetWorld(); //David
