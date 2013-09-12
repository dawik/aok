/*
File: net.c
By: David Wikmans
Rev. 110506 Tobias Hultgren
Created 21 april, 2011.
*/

#include "init.h"
#include "header.h"
#include "net.h"

/* Network related functions -- David */
int cmdsend(int index, int x, int y, char *data, char *data2) {
	char buffer[PACKETSIZE];
	snprintf(buffer, PACKETSIZE,"%i#%i#%i#%s#%s#",index,x,y,data,data2);
	if (DEBUG) printf("sending %s\n",buffer);
	int length = strlen(buffer)+1;
	if (SDLNet_TCP_Send(sock,buffer,length)<length) {
		fprintf(stderr,"%s",SDLNet_GetError());
		return -1;
	} else
		return 0;
}

/* Parse instruction type string -- David */
struct instruction cmdparse (struct instruction i, char *in) {
	int x = 0,y = 0;
	char *ptr = strtok(in,"#");
	strcpy(i.data[x++],ptr);
	for (; y<6; y++) {
		ptr = strtok(NULL,"#");
		if (ptr!=NULL)
			strcpy(i.data[x++],ptr);
	}
	return i;
}

/* Parse and update player list -- David */
void UpdateUserList(char *string) {
	int j,x, y;
	char *ptr;
	ptr = strtok(&string[2],"#");
	j = (int) atoi(ptr);
	for (x = 0; x < j ; x++) {
		y = atoi(strtok(NULL,"#"));
		ptr = strtok(NULL,"#");
		strcpy(connectedClient[y], ptr);
	}
}

/* Split big update string into smaller pieces and execute specific parsers -- David */
void ParseUpdate(char* string) {
	char *ptr1, *ptr2;
	char segment[3][PACKETSIZE];
	int x;
		for (x = 0; x < 3; x++)
			strcpy(segment[x],string);
		ptr1 = strchr(segment[0],'R');
		UpdateResources(ptr1);
		ptr1 = strchr(segment[1],'H');
		ptr2 = ptr1;
		while (*ptr2 != 0) {
			if (*ptr2 == 'R')
				*ptr2 = '\0';
			ptr2++;
		}
		UpdateHealth(ptr1);
		ptr1 = strchr(segment[0],'M');
		ptr2 = ptr1;
		while (*ptr2 != 0) {
			if (*ptr2 == 'H')
				*ptr2 = '\0';
			ptr2++;
		}
		MoveUnits(ptr1);
}

/* UDP Listening thread -- David */
int udpThread(void *data)
{
	struct shared_data* d = (struct shared_data*) data;
	int udp_port;
	char *ptr1, buffer[PACKETSIZE];
	UDPpacket *packet;


	/* Find available UDP port and open it */
	for (udp_port = 30000 ; udp_port < 30020 ; udp_port++) {
		usock = SDLNet_UDP_Open(udp_port);
		if (usock) {
			if(DEBUG) printf("Assigned UDP port %i\n", SDL_SwapBE16(udp_port));
			break;
		} else {
			SDLNet_UDP_Close(usock);
		}
	}

	/* Send handshake */
	cmdsend(HANDSHAKE,udp_port,0,d->name,NULL);

	/* Make space for the packet */
	if (!(packet = SDLNet_AllocPacket(1024)))
	{
		fprintf(stderr, "Out of memory? %s\n", SDLNet_GetError());
		exit(999);
	}
	while (running) {
		if (SDLNet_UDP_Recv(usock, packet))
		{
			strcpy(buffer,(char*)packet->data);
			if (strlen(buffer)>MIN_STR_LEN) {
				if (DEBUG) fprintf(stdout,"UDP PACKET CONTAINS: %s\n",(char *) packet->data);
				/* If UDP-test packet is received reply that UDP is OK to use */
				if (buffer[0]=='!') {
					ptr1 = &buffer[2];
					ptr1 = strtok(ptr1,"!");
					if (strcmp(ptr1,"UDPTEST")) {
						cmdsend(UDP_TEST,client_id,0,NULL,NULL);
					}
				}
				/* Parse big update packet */
				else if (packet->data[0]=='M') {
					ParseUpdate(buffer);
				}
			}
		}
	}
	return 0;
}


/* TCP Listening thread -- David */
int netThread(void *data)
{
	int len, x, y, j, count;
	struct instruction i;
	int position[2], unit_id, unit_owner,unit_class;
	char buffer[PACKETSIZE], *ptr1;

	while(running)
	{
		/* Error handling */
		len=SDLNet_TCP_Recv(sock, buffer, PACKETSIZE);
		if (len<=0) {
			if (DEBUG) 		fprintf(stderr,"Error receiving packet-Server crash?\n");
			running=0;
		}
		else {
			if (DEBUG) fprintf(stderr,"Received: %s\n", buffer);
			if (buffer[0]=='U') {
				UpdateUserList(buffer);
			} else if (buffer[0]=='M') {
				ParseUpdate(buffer);
			} else {
				/* Interpret instructions */
				i=cmdparse(i, buffer);
				switch (strtol(i.data[0],&ptr1,0))
				{
					case HANDSHAKE:
						client_id=atoi(i.data[1]);
						if(DEBUG) fprintf(stdout,"Assigned as client %i\n",client_id);
						resources=atoi(i.data[2]);
						strcpy(settings.mapname,i.data[3]);
						LoadMap();
						for(j=CHAT_MESSAGES_BUFFER-1; j>=0; j--) strcpy(messagebuffer[j+1<CHAT_MESSAGES_BUFFER?j+1:CHAT_MESSAGES_BUFFER-1],messagebuffer[j]);
						snprintf(messagebuffer[0], MESSAGESIZE, " >> Joined the game as \"%s\" ID %d", settings.username, client_id);
						break;
					case CHAT:
						for(j=CHAT_MESSAGES_BUFFER-1; j>=0; j--) strcpy(messagebuffer[j+1<CHAT_MESSAGES_BUFFER?j+1:CHAT_MESSAGES_BUFFER-1],messagebuffer[j]);
						snprintf(messagebuffer[0], MESSAGESIZE, "[%s] %s", i.data[3],i.data[4]);
						if (DEBUG) printf("MESSAGE %s FROM %s\n", i.data[4] ,i.data[3]);
						break;
					case SPAWN_UNIT:
						position[0]=strtol(i.data[1],&ptr1,0);
						position[1]=strtol(i.data[2],&ptr1,0);
						unit_id = strtol(i.data[3],&ptr1,0);
						unit_owner=strtol(i.data[4],&ptr1,0);
						unit_class=strtol(i.data[5],&ptr1,0);
						SpawnUnit(position, unit_class, unit_id, unit_owner);
						if (DEBUG) fprintf(stderr,"spawning unit %i owner %i\n",unit_id,unit_owner);
						break;
					case DESTROY_UNIT:
						unit_id = strtol(i.data[1],&ptr1,0);
						KillUnit(unit_id);
						if(unit_id == client_id) PlayGameSound(UNIT_LOST);
						break;
					case RESET_WORLD:
						ResetWorld();
						if (DEBUG) fprintf(stdout,"Map and units reset\n");
						break;
					case START_COUNTDOWN:
						if (DEBUG) fprintf(stdout,"Beginning countdown\n");
						endgameStatus = -1;
						ResetWorld();
						position[0]=strtol(i.data[1],&ptr1,0);
						position[1]=strtol(i.data[2],&ptr1,0);
						/* Initiate countdown */
						while (countdown != -1) SDL_Delay(1);
						countdown = SDL_GetTicks();
						count = SDL_GetTicks();
						while (SDL_GetTicks() < count + 5000);
						offset[0] = position[0]/2 + position[1]/2 - WINDOWWIDTH/TILEDIAGONAL/2 + 1; //Centers camera over the player spawn point
						offset[1] = position[0]/2 - position[1]/2 - WINDOWHEIGHT/TILEDIAGONAL + 1;
						break;
					case VICTORY:
						PlayGameSound( WON );
						endgameStatus = 1;
						showEndgameStatus = true;
						for(x=0; x<MAP_WIDTH; x++)
							for(y=0; y<MAP_HEIGHT; y++)
								map[x][y].seen = 1; //Makes all tiles seen
						break;
					case FAIL:
						PlayGameSound( LOST );
						endgameStatus = 0;
						showEndgameStatus = true;
						for(x=0; x<MAP_WIDTH; x++)
							for(y=0; y<MAP_HEIGHT; y++)
								map[x][y].seen = 1; //Makes all tiles seen
						break;
				}
			}
		}
	}

	return 0;
}
