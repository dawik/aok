/*
 * File:   routines.c
 * Author: Projektgrupp 4
 * Created by: Tobias Hultgren
 * Revisioned by: David Wikmans
 *
 * Created on April 2, 2011
 */

#include "header.h"
#include "net.h"


void MoveUnits(char* string)
{
	/* Parser and Movement -- David */
	int x, unit_id, position[2];
	char *ptr1, *ptr2, *endptr;
	ptr1 = (char *) &string[2];
	ptr1 = strtok(ptr1,"#");
	for (x = 0; x < strtol(ptr1,&endptr,0); x++) {
		ptr2 = strtok(NULL,"#");
		unit_id=strtol(ptr2,&endptr,0);
		ptr2 = strtok(NULL,"#");
		position[0]=strtol(ptr2,&endptr,0);
		ptr2 = strtok(NULL,"#");
		position[1]=strtol(ptr2,&endptr,0);
		map[unit[unit_id].position[0]][unit[unit_id].position[1]].unit=NULL;

		//Updates unit facing -- Tobias
		if (position[1] > unit[unit_id].position[1]) //N, NE, E
		{
			if (position[0] < unit[unit_id].position[0]) unit[unit_id].facing = NORTH; //N, NW, W
			else if (position[0] > unit[unit_id].position[0]) unit[unit_id].facing = EAST; //E, SE, S
			else unit[unit_id].facing = NORTHEAST; //NE, SW
		}
		else if (position[1] < unit[unit_id].position[1]) // W, SW, S
		{
			if (position[0] < unit[unit_id].position[0]) unit[unit_id].facing = WEST; //N, NW, W
			else if (position[0] > unit[unit_id].position[0]) unit[unit_id].facing = SOUTH; //E, SE, S
			else unit[unit_id].facing = SOUTHWEST; //NE, SW
		}
		else //NW, SE
		{
			if (position[0] < unit[unit_id].position[0]) unit[unit_id].facing = NORTHWEST; //N, NW, W
			else if (position[0] > unit[unit_id].position[0]) unit[unit_id].facing = SOUTHEAST; //E, SE, S
		}

		unit[unit_id].fromcoord[0] = unit[unit_id].destcoord[0];
		unit[unit_id].fromcoord[1] = unit[unit_id].destcoord[1];
		unit[unit_id].coord[0] = unit[unit_id].destcoord[0];
		unit[unit_id].coord[1] = unit[unit_id].destcoord[1];
		unit[unit_id].fromtime = SDL_GetTicks();

		unit[unit_id].position[0]=position[0];
		unit[unit_id].position[1]=position[1];

		map[position[0]][position[1]].unit=&unit[unit_id];
	}
}

void UpdateResources(char* string)
{
	int owner_id;
	char *ptr, *endptr;
	ptr = (char *) &string[2];
	owner_id=strtol(ptr,&endptr,0);
	ptr = (char *) &string[4];
	if (owner_id == client_id) {
		resources = strtol(ptr,&endptr,0);
	}
}

void UpdateHealth(char* string)
{
	int x, unit_id, health;
	char *ptr1, *ptr2, *endptr;
	ptr1 = (char *) &string[2];
	ptr1 = strtok(ptr1,"#");
	for (x = 0; x < strtol(ptr1,&endptr,0); x++) {
		ptr2 = strtok(NULL,"#");
		unit_id=strtol(ptr2,&endptr,0);
		ptr2 = strtok(NULL,"#");
		health=strtol(ptr2,&endptr,0);
		unit[unit_id].health = health;
		if( unit_id == client_id ) PlayGameSound( UNDER_ATTACK );
	}
}

int SpawnUnit(int* position, int class, int unit_id, int owner)
{
	if(owner == client_id) population++;
		if(DEBUG) fprintf(stderr, "Spawning unit %i of class %i - owner %i\n", unit_id, class, owner);
	unit[unit_id].class = class;
	unit[unit_id].position[0] = position[0];
	unit[unit_id].position[1] = position[1];
	unit[unit_id].destination[0] = -1;
	unit[unit_id].destination[1] = -1;
	unit[unit_id].selected = false;
	unit[unit_id].waitto = 0;
	unit[unit_id].fromtime = 0;
	unit[unit_id].status = IDLE;
	unit[unit_id].id = unit_id;
	unit[unit_id].facing = SOUTH;
	unit[unit_id].target = -1;
	unit[unit_id].group = 0;
	unit[unit_id].owner = owner;
	unit[unit_id].nextattack = 0;
	unit[unit_id].width = unit_class[class].width;
	unit[unit_id].height = unit_class[class].height;
	unit[unit_id].speed = unit_class[class].speed;
	unit[unit_id].health = unit_class[class].health;
	unit[unit_id].maxhealth = unit_class[class].health;
	unit[unit_id].damage = unit_class[class].damage;
	unit[unit_id].range = unit_class[class].range;
	unit[unit_id].attackspeed = unit_class[class].attackspeed;
	unit[unit_id].sight = unit_class[class].sight;
	unit[unit_id].canwork = unit_class[class].canwork;
	map[position[0]][position[1]].unit = &unit[unit_id];
	if(DEBUG) fprintf(stdout,"Spawning unit: %d at %d,%d\n",unit[unit_id].id, unit[unit_id].position[0], unit[unit_id].position[1]);
	if(totalUnits < unit_id) totalUnits = unit_id;
	return unit_id;
}

void KillUnit(int unit_id)
{
	if(unit[unit_id].owner == client_id)
	{
		population--; //Decrement population by 1 when killing own unit
		if(population < 0) population = 0;
	}

	map[unit[unit_id].position[0]][unit[unit_id].position[1]].unit = NULL;
	unit[unit_id].class = 0;
	unit[unit_id].selected = 0;
	if(DEBUG) fprintf(stderr,"Killing unit %i coords %i,%i\n",unit_id,unit[unit_id].position[0],unit[unit_id].position[1]);
}

int SetTarget(int* mPos)
{
	mPos[0] += offset[0] * TILEDIAGONAL;
	mPos[1] += offset[1] * TILEDIAGONAL/2;

	int i, target = -1;
	for(i = 0; i <= totalUnits; i++) //Note the selected target
	{
		if(unit[i].class != 0)
		{
			if(    mPos[0] > unit[i].coord[0] - unit[i].width*TILEDIAGONAL/2
					&& mPos[1] > unit[i].coord[1] - unit[i].height*TILEDIAGONAL/2
					&& mPos[0] < unit[i].coord[0] + unit[i].width*TILEDIAGONAL/2
					&& mPos[1] < unit[i].coord[1] + unit[i].height*TILEDIAGONAL/2)
			{
				target = i;
					if(DEBUG) fprintf(stdout,"Targeted unit %i for", target);
						break;
			}
		}

	}
	if(target != -1) //Only assign selected units the target if it was found
		for(i = 0; i <= totalUnits; i++)
			if(unit[i].selected)
			{
				unit[i].target = target;
				if(DEBUG) fprintf(stdout," %i", i);
			}
	if(DEBUG && target != -1) fprintf(stdout,"\n");

	//Plays ATTACK or FOLLOW sound
	if(target != -1)
	{
		if(unit[target].owner == client_id) PlayGameSound( FOLLOW );
		else PlayGameSound( ATTACK );
	}

	return target; //Returns target ID if successful, otherwise -1
}

void SetSelectedGroup()
{
	int i;
	if (!shiftDown){
		for(i=0;i<=totalUnits;i++) unit[i].selected = false;
	}

	if(DEBUG) fprintf(stdout,"Selected units: ");

	for(i=0;i<=totalUnits;i++)
	{
		if(    unit[i].coord[0] + unit[i].width/2*TILEDIAGONAL > boxStart[0]
				&& unit[i].coord[0] - unit[i].width/2*TILEDIAGONAL < boxEnd[0]
				&& unit[i].coord[1] > boxStart[1]
				&& unit[i].coord[1] - unit[i].height*TILEDIAGONAL < boxEnd[1]
				&& unit[i].owner == client_id)
		{
			unit[i].selected = true;
			if(DEBUG) fprintf(stdout,"%d ", unit[i].id);
		}
	}
	if(DEBUG) fprintf(stdout,"\n");
}

int SetSelected(int* mPos)
{
	mPos[0] += offset[0] * TILEDIAGONAL;
	mPos[1] += offset[1] * TILEDIAGONAL/2;

	int i;
	if (!shiftDown){
		for(i=0;i<=totalUnits;i++) unit[i].selected = false;
	}

	for(i=0;i<=totalUnits;i++)
	{
		if(    mPos[0] >  unit[i].coord[0] - unit[i].width*TILEDIAGONAL/2
				&& mPos[1] >  unit[i].coord[1] - unit[i].height*TILEDIAGONAL/2
				&& mPos[0] <  unit[i].coord[0] + unit[i].width*TILEDIAGONAL/2
				&& mPos[1] <  unit[i].coord[1] + unit[i].height*TILEDIAGONAL/2
				&& unit[i].owner == client_id)
		{
			unit[i].selected = true;
			if(DEBUG) fprintf(stderr,"Selected unit: %d\n", unit[i].id);
			return unit[i].id;
		}
	}
	return false;
}

int ClickHUDButton(int* mPos)
{
	int i;
	for(i = 0; i < MAX_TEXTURES_HUD; i++)
	{
		if(    mPos[0] > WINDOWWIDTH - (i+1)*(HUD_HEIGHT + HUD_ICON_PADDING)
				&& mPos[0] < WINDOWWIDTH - (i+1)*(HUD_HEIGHT + HUD_ICON_PADDING) + HUD_HEIGHT - HUD_ICON_PADDING
				&& mPos[1] > WINDOWHEIGHT - HUD_HEIGHT + HUD_ICON_PADDING
				&& mPos[1] < WINDOWHEIGHT - HUD_ICON_PADDING)
		{
			if(DEBUG) fprintf(stdout,"Clicked menu item %i\n", i);
				if(population >= MAX_POPULATION)
				{
					//PlayGameSound( TOO_MANY_UNITS );
					return -1;
				}
			cmdsend(99,client_id,i+1," "," ");
			return i;
		}
	}
	return false;
}

int SendUnits(int* position, int target_unit)
{
	int i, unit_count = 0, walk = false;
	char temp[8], temp_str[2048], buffer[PACKETSIZE];
	GetIsometricPoint(position);
	temp[0] = '\0';
	temp_str[0] = '\0';
	for(i=0; i<=totalUnits; i++)
		if(unit[i].selected && unit[i].owner==client_id)
		{
			unit[i].target = target_unit;
			unit_count++;
			sprintf(temp,"%i#",i);
			strcat(temp_str, temp);

			if( walk == false && map[position[0]][position[1]].walkable == 1 )
			{
				int dx,dy,dist;
				dx = abs(position[0]-unit[i].position[0]);
				dy = abs(position[1]-unit[i].position[1]);
				dist = sqrt(dx*dx + dy*dy);
				if ( dist-0 >= 0 ) walk = true;
			}
		}

	if ((position[0] >= 0 && position[0] <= MAP_WIDTH) && (position[1] >= 0 && position[1] <= MAP_HEIGHT)) {
		if (target_unit < 0)
		{
			snprintf(buffer,PACKETSIZE,"M#%i#%i#%i#",unit_count,position[0],position[1]);
		}

		else if (unit[target_unit].owner == client_id)
		{
			snprintf(buffer,PACKETSIZE,"F#%i#%i#",unit_count,target_unit);
		}

		else if (unit[target_unit].owner != client_id)
		{
			snprintf(buffer,PACKETSIZE,"A#%i#%i#",unit_count,target_unit);
		}

		strcat(buffer,temp_str);
		if (unit_count > 0) {

			/*If tile is walkable, move command is in maxwalk range and the command issued*/
			if ( walk == true && target_unit == -1) PlayGameSound ( MOVE );
			else if(target_unit == -1) PlayGameSound ( CANNOT );

			if(SDLNet_TCP_Send(sock, buffer, strlen(buffer)+1) < strlen(buffer)+1){
				fprintf(stderr,"%s",SDLNet_GetError());
				return false;
			}
			return true;
		}
		return false;
	}
	return false;
}

void GetIsometricPoint(int* position)
{
	float isometric[2], temp[2];
	temp[0] = (float)position[0];
	temp[1] = (float)position[1];
	temp[0] += offset[0] * TILEDIAGONAL;
	temp[1] += offset[1] * (TILEDIAGONAL/2);
	temp[0] /= (TILEDIAGONAL / 2);
	temp[1] /= (TILEDIAGONAL / 4);
	isometric[0] = (temp[0] + temp[1]) / 2 - 0.5;
	isometric[1] = (temp[0] - temp[1]) / 2 - 0.5;
	position[0] = (int)isometric[0];
	position[1] = (int)isometric[1];
	return;
}

int SetDestination(int* position)
{
	GetIsometricPoint(position);
	int i, unit_count = 0,walk = false;
	char temp[8], temp_str[2048], buffer[PACKETSIZE];
	temp[0] = '\0';
	temp_str[0] = '\0';
	if ((position[0] >= 0 && position[0] <= MAP_WIDTH) && (position[1] >= 0 && position[1] <= MAP_HEIGHT))
	{
		for(i=0; i<=totalUnits; i++)
			if(unit[i].selected && unit[i].owner==client_id)
			{
				unit[i].target = -1;
				unit_count++;
				sprintf(temp,"%i#",i);
				strcat(temp_str, temp);

				/* If there is a max limit set how far a unit can walk (because of server limiting pathing)
					 This function tests if one or more units are within this range. Continues on line 242.
					 (maxwalk = maximum range unit is allowed to move) // Written by Emil L*/
				if( walk == false && map[position[0]][position[1]].walkable == 1 )
				{
					int dx,dy,dist;
					dx = abs(position[0]-unit[i].position[0]);
					dy = abs(position[1]-unit[i].position[1]);
					dist = sqrt(dx*dx + dy*dy);
					if ( dist-0 >= 0 ) walk = true;
				}
			}
		if (unit_count == 0)
			return false;
		else
			if ( walk == true ) PlayGameSound ( MOVE );
			else PlayGameSound ( CANNOT );

		snprintf(buffer,PACKETSIZE,"M#%i#%i#%i#",unit_count,position[0],position[1]);
		strcat(buffer,temp_str);
		printf("sending %s\n",buffer);
		int len = strlen(buffer)+1;
		if(SDLNet_TCP_Send(sock, buffer, len) < len){
			fprintf(stderr,"%s",SDLNet_GetError());
			return false;
		}
		return true;
	} else {
		return false;
	}
}

void SetControlGroup(int id)
{
	int i;

	for(i = 0; i <= totalUnits; i++) //Remove all units from the specified control group
		if(unit[i].group == id)
			unit[i].group = 0;

	for(i = 0; i <= totalUnits; i++) //Add the selected units to the specified control group
		if(unit[i].selected)
			unit[i].group = id;
}

void GetControlGroup(int id)
{
	int i;
	if (!shiftDown){ //Deselect all units if shift isn't down
		for(i = 0; i <= totalUnits; i++) unit[i].selected = false;
	}

	for(i = 0; i <= totalUnits; i++)
		if(unit[i].group == id)
		{
			unit[i].selected = true;
				if(lastSelectedGroup == id) //If the player selected the same group twice, center the camera over it
				{
					offset[0] = unit[i].position[0]/2 + unit[i].position[1]/2 - WINDOWWIDTH/TILEDIAGONAL/2 + 1; //Centers camera over the selected group
					offset[1] = unit[i].position[0]/2 - unit[i].position[1]/2 - WINDOWHEIGHT/TILEDIAGONAL + 1;
				}
		}
	lastSelectedGroup = id;
}
