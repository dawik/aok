/*
file:net.c
created:2011-04-23
<wikmans@kth.se>
revisioned by:
*/

#include "header.h"
#include "init.h"
#include "net.h"

/* functions */

void shake (struct instruction i, void *data, IPaddress *remoteIP, int id) {
  struct shared_data* d = (struct shared_data*) data;
	number_of_clients++;
	fprintf(stdout,"Hello %s! clients: %i\n",i.data[3], number_of_clients);
	/* assign socket to client and store IP + UDP port*/
	d->clients[id].sock = sock;
	d->clients[id].ip.host = remoteIP->host;
	d->clients[id].ip.port = SDL_SwapBE16(atoi(i.data[1]));
	/* register name of client */
	strcpy(d->clients[id].name,i.data[3]);
	/* return client id number and resource amount */
	cmdsend(HANDSHAKE,id,resources[id],mapname,NULL,sock);
	/* test if udp works for client */
	udptest(d->client_pool,d->clients,id);
	/* add socket to array */
	SDLNet_TCP_AddSocket(d->set, d->clients[id].sock);
	/* occupy player slot */
	d->client_pool[id] = 0;
}

void send_reset(int *pool, struct client_t *clients) {
	char buffer[PACKETSIZE];
	snprintf(buffer,PACKETSIZE,"9999#0#0#0#0#");
	broadcast(pool,clients,buffer);
	ResetWorld();
	if (DEBUG) fprintf(stdout,"Client and Server world reset\n");
}


void dropclient(void *data, int id) {
	struct shared_data* d = (struct shared_data*) data;
	SDLNet_TCP_DelSocket(d->set,d->clients[id].sock);
	d->clients[id].udp = 0;
	d->client_pool[id]=-1;
	number_of_clients--;
	fprintf(stdout, "Client %s dropped clients:%i\n",d->clients[id].name,number_of_clients);
	strcpy(d->clients[id].name," ");
}

struct instruction cmdparse (struct instruction i, char *in, int id) {
	int x=0,y=0;
	char *ptr = strtok(in,"#");
	strcpy(i.data[x++],ptr);
	for (;y<4;y++) {
		ptr = strtok(NULL,"#");
		if (ptr!=NULL)
			strcpy(i.data[x++],ptr);
	} i.id=id;
	return i;
}

void parse_move (char *string) {
	char *unit_ptr;
	int x, units_to_move, position[2];
	unit_ptr = strtok(&string[2],"#");
	units_to_move = atoi(unit_ptr);
	unit_ptr = strtok(NULL,"#");
	position[0] = atoi(unit_ptr);
	unit_ptr = strtok(NULL,"#");
	position[1] = atoi(unit_ptr);
	for (x = 0; x < units_to_move ; x++) {
		unit_ptr = strtok(NULL,"#");
		unit[atoi(unit_ptr)].target = -1;
		SetDestination(position,atoi(unit_ptr));
	}
}

void parse_target (char *string) {
	char *unit_ptr;
	int x, units_to_move, target_unit;
	unit_ptr = strtok(&string[2],"#");
	units_to_move = atoi(unit_ptr);
	unit_ptr = strtok(NULL,"#");
	target_unit = atoi(unit_ptr);
	for (x = 0; x < units_to_move ; x++) {
		unit_ptr = strtok(NULL,"#");
		unit[atoi(unit_ptr)].target = target_unit;
	}
}

int cmdsend(int index, int x, int y, char *data, char *data2, TCPsocket sock) {
	char buffer[PACKETSIZE];
	sprintf(buffer,"%i#%i#%i#%s#%s",index,x,y,data,data2);
	printf("sending %s\n",buffer);
	int length = strlen(buffer)+1;
	if (SDLNet_TCP_Send(sock,buffer,length)<length) {
		fprintf(stderr,"%s",SDLNet_GetError());
		return -1;
	} else
		return 0;
}

void tcpcast (int *pool, struct client_t *clients, char *msg) {
	int x=0;
	/* send message */
	for (;x<maxclients;x++) {
		if (pool[x]>=0 && clients[x].udp == 0) {
			SDLNet_TCP_Send(clients[x].sock,msg,strlen(msg)+1);
		}
	}
}

void broadcast (int *pool, struct client_t *clients, char *msg) {
	int x=0;
	/* send message */
	for (;x<maxclients;x++) {
		if (pool[x]>=0) {
			SDLNet_TCP_Send(clients[x].sock,msg,strlen(msg)+1);
SDL_Delay(50);
		}
	}
}

void udptest (int *pool, struct client_t *clients, int id) {
	UDPpacket *packet;
	packet=SDLNet_AllocPacket(1024);
	int sent_packets;
	if (!packet)
		printf("Cant allocate memory for packet: %s\n",SDLNet_GetError());
	else {
		strcpy((char *)packet->data,"!UDPTEST!");
		packet->len = strlen("!UDPTEST!")+1;
	}
	packet->address.host = clients[id].ip.host;
	packet->address.port = clients[id].ip.port;
	sent_packets = SDLNet_UDP_Send(usock,-1 ,packet);
	if (DEBUG) fprintf(stdout,"udpstr: %s remoteip: %i remoteport: %i sent packets: %i\n",packet->data,SDLNet_Read32 (&packet->address.host), packet->address.port, sent_packets);
	SDLNet_FreePacket(packet);
	packet=NULL;
}


void ubroadcast (int *pool, struct client_t *clients, char *msg) {
	int x=0;
	UDPpacket *packet;
	packet=SDLNet_AllocPacket(512);
	int sent_packets;
	if (!packet)
		printf("Cant allocate memory for packet: %s\n",SDLNet_GetError());
	else {
		strcpy((char *)packet->data,msg);
		packet->len = strlen(msg)+1;
	}
	/* send message */
	for (;x<maxclients;x++) {
		if (pool[x]>=0 && clients[x].udp == 1) {
			packet->address.host = clients[x].ip.host;
			packet->address.port = clients[x].ip.port;
			sent_packets = SDLNet_UDP_Send(usock,-1 ,packet);
			if (DEBUG) fprintf(stdout,"udpstr: %s remoteip: %s remoteport: %i sent packets: %i\n",packet->data,SDLNet_ResolveIP(&packet->address), packet->address.port, sent_packets);
		}
	}
	SDLNet_FreePacket(packet);
	packet=NULL;
}

void update_stats (int *client_pool, struct client_t *clients, char *string) {

	char buffer[PACKETSIZE], temp[64];
	int x;
	for (x=0; x < maxclients; x++) {
		if (client_pool[x] >= 0) {
			strcpy(buffer,string);
			sprintf(temp,"R#%i#%i#",x,resources[x]);
			strcat(buffer,temp);
			if (clients[x].udp == 1) {
				UDPpacket *packet;
				packet=SDLNet_AllocPacket(512);
				strcpy((char *)packet->data,buffer);
				packet->len = strlen(buffer)+1;
				packet->address.host = clients[x].ip.host;
				packet->address.port = clients[x].ip.port;
				SDLNet_UDP_Send(usock,-1 ,packet);
				SDLNet_FreePacket(packet);
				packet=NULL;
			} else {
				SDLNet_TCP_Send(clients[x].sock,buffer,strlen(buffer)+1);
			}
			if (DEBUG) fprintf(stdout,"Sent string %s to client: %i ip: %i port: %i\n",buffer,x, clients[x].ip.host, clients[x].ip.port);
		}
	}
}

void printClients(int *pool, struct client_t *clients) {
	int x=0;
	for (;x<maxclients;x++)
		if (pool[x] >= 0)
			printf("ID: %i Name: %s Host: %s Port:%d Ready: %i\n", x, clients[x].name, SDLNet_ResolveIP(&clients[x].ip),
					SDLNet_Read16(&clients[x].ip.port), PLAYER_READY[x]);
}

int getslot(int *pool) {
	int x=0;
	for (;x<maxclients;x++) {
		if (pool[x]<0) {
			return x;
		}
	}
	return -1;
}

SDLNet_SocketSet create_sockset(TCPsocket server, UDPsocket usock, struct client_t *clients, int *client_pool)
{
	static SDLNet_SocketSet set=NULL;
	int x=0;
	set=SDLNet_AllocSocketSet(maxclients);
	SDLNet_TCP_AddSocket(set,server);
	SDLNet_UDP_AddSocket(set,usock);
	for(;x<maxclients;x++) {
		if (client_pool[x]>=0)
			SDLNet_TCP_AddSocket(set,clients[x].sock);
	}
	return(set);
}
