/*
File: sessions.c
By: David Wikmans
Created 13 april, 2011.
 */

#include "init.h"
#include "header.h"
#include "net.h"

int sessionThread(void *data) {
	struct shared_data* d = (struct shared_data*) data;
	struct instruction i;
	int socketvector, available_slot, x=0, y=0, position[2], unit_id, owner_id, class_id;
	char temp[64], buffer[PACKETSIZE];
	IPaddress ip, *remoteIP;
	TCPsocket server;

	SDLNet_Init();
	SDLNet_ResolveHost(&ip, NULL, 6001);
	/* open UDP socket at available port */
	usock = SDLNet_UDP_Open(0);
	/* listening at localhost port 6000 */
	if (!(server = SDLNet_TCP_Open(&ip))) {
		fprintf(stderr,"cant bind port\n");
		exit(1);
	}

	InitClients(d->clients);

	if (DEBUG) fprintf(stdout, "Session thread started!\n");

	while (running) {
		d->set=create_sockset(server,usock,d->clients,d->client_pool);
		socketvector=SDLNet_CheckSockets(d->set, (Uint32)-1);
		if(socketvector==-1)
		{
			printf("%s\n",SDLNet_GetError());
			break;
		}
		/* activity on socket array  */
		if(!socketvector)
			continue;
		/* if incoming connection on listening port */
		if(SDLNet_SocketReady(server))
		{
			/* establish session */
			if (GAME != GAME_ON && number_of_clients <= total_spawnpoints)
				sock=SDLNet_TCP_Accept(server);
			/* success */
			if(sock)
			{
				available_slot=getslot(d->client_pool);
				struct instruction i;
				if (SDLNet_TCP_Recv(sock, buffer, PACKETSIZE) > 0) { // incoming packet of PACKETSIZE bytes
					i=cmdparse(i,buffer,available_slot);
					if (atoi(i.data[0])==HANDSHAKE) {
						remoteIP = SDLNet_TCP_GetPeerAddress(sock);
						shake(i,d,remoteIP,available_slot);
					}
				}
			} else {
				/* failed handshake: close session */
				SDLNet_TCP_Close(sock);
			}
		}

		/* monitor established tcp sessions */
		for(x=0; socketvector && x<maxclients; x++)
		{
			/* activity on socket */
			if (d->client_pool[x]!=-1) {
				if(SDLNet_SocketReady(d->clients[x].sock))
				{
					/* parse instruction */
					if (SDLNet_TCP_Recv(d->clients[x].sock,buffer,PACKETSIZE)) {
						if (DEBUG)					printf("Received string %s\n",buffer);
						/* parse and exec movement request */
						if (buffer[0]=='M' && strlen(buffer) > minpacket) {
							parse_move(buffer);
							/* parse attack and follow request */
						} else if ((buffer[0]=='A' || buffer[0]=='F') && strlen(buffer) > minpacket) {
							parse_target(buffer);
						} else {
							i=cmdparse(i,buffer,x);
							/* interpret instruction */
							switch (atoi(i.data[0])) {
								case NONE:
									break;
								case CHAT: 
									/* set player ready -- toggles */
									if (!strcmp(i.data[3],"/ready")) {
										owner_id = atoi(i.data[2]);
										if (PLAYER_READY[owner_id] < 0) {
											snprintf(buffer,PACKETSIZE,"1#0#0#%s#%s is ready#","Server",d->clients[owner_id].name);
											PLAYER_READY[owner_id] = 1;
										} else {
											snprintf(buffer,PACKETSIZE,"1#0#0#%s#%s is NOT ready#","Server",d->clients[owner_id].name);
											PLAYER_READY[owner_id] = -1;
										}
										broadcast(d->client_pool,d->clients,buffer);
									} else {
										/* broadcast chat */
										snprintf(buffer,PACKETSIZE,"1#0#0#%s#%s",d->clients[atoi(i.data[2])].name,i.data[3]);
										broadcast(d->client_pool,d->clients,buffer);
										fprintf(stdout,"%s: %s\n",d->clients[atoi(i.data[1])].name,i.data[3]);
										if (DEBUG) fprintf(stdout,"%s\n",buffer);
									}
									break;
								case REQUEST_USERS:
									/* compose user string */
									sprintf(buffer,"U#%i#",number_of_clients);
									for (y = 0; y < maxclients ; y++) {
										sprintf(temp,"%i#%s#",y,d->clients[y].name);
										strcat(buffer,temp);
									}
									SDLNet_TCP_Send(d->clients[atoi(i.data[1])].sock,buffer,strlen(buffer)+1);
									if (DEBUG) fprintf(stderr,"sending tabstring %s\n",buffer);
									break;
								case SPAWN_UNIT: 
									/* create unit */
									owner_id=atoi(i.data[1]);
									class_id=atoi(i.data[2]);
									if (class_id > UNIT_CLASSES)
										break;
									position[0]=playerStart[owner_id].x;
									position[1]=playerStart[owner_id].y;
									if (resources[owner_id] - unit_class[class_id].price >= 0) {
										unit_id=SpawnUnit(position,class_id,owner_id);
										sprintf(buffer,"%i#%i#%i#%i#%i#%i",SPAWN_UNIT,playerStart[owner_id].x,playerStart[owner_id].y,unit_id, owner_id ,class_id);
										broadcast(d->client_pool,d->clients,buffer);
										if (DEBUG) fprintf(stdout,"sending string %s\n",buffer);
										break;
									} else
										break;
									/* enable udp for said client */
								case UDP_TEST:
									d->clients[atoi(i.data[1])].udp = 1;
									if (DEBUG) fprintf(stdout,"Client %i - %s UDP ENABLED\n",atoi(i.data[1]),d->clients[atoi(i.data[1])].name);
									break;
								default:
									if (DEBUG) fprintf(stdout,"client %i sent instruction %i\n",x,atoi(i.data[0]));
							}
						}
					} else {
						/* no activity --> remove client */
						dropclient(d,x);
					}
				}
				/* release previous socket array */
				if (!d->set)
					free(d->set);
			}
		}
	}
	SDLNet_FreeSocketSet(d->set);
	return 0;
}

