/*
 * File:   pathfinding.h
 * Author: Projektgrupp 4
 * Created by: Christian Samuelsson
 * Revisioned by:
 *
 * Created on April 11, 2011
 */

#define OPEN 1
#define CLOSED 2
#define PATH 3
#define SCAN_LIMIT 3000
struct Point infolist[SCAN_LIMIT]; //Christoffer

void SetInfo(int info[MAP_WIDTH][MAP_HEIGHT], struct Point p, int set); //Christoffer

void GetClosest(struct Point *closest, struct Point source, int (*info)[MAP_HEIGHT]); //Christoffer

struct Point Minimum(int (*info)[MAP_HEIGHT] ,int (*G)[MAP_HEIGHT]); //Christoffer
