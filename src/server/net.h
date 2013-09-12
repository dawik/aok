enum protocol { //David
	NONE = 0,
	CHAT = 1,
	MOVE_TO = 2,
	REQUEST_USERS = 80,
  SPAWN_UNIT = 99,
	DESTROY_UNIT = 100,
	HANDSHAKE = 1000,
	UDP_TEST = 1001,
	RESET_WORLD = 9999,
	START_COUNTDOWN = 12345,
	VICTORY = 20000,
	FAIL = 20001
} instruction_codes;

enum game_status { //David
WARMUP,
GAME_ON,
COUNTDOWN
};

struct client_t { //David
        char name[32];
        int id, udp, receiving;
		int resources;
        TCPsocket sock;
        SDLNet_SocketSet set_l;
        IPaddress ip;
};

struct shared_data { //David
	int client_pool[maxclients];
	struct client_t clients[maxclients];
  SDLNet_SocketSet set;
};

struct instruction { //David
	int id;
	char data[5][256];
};

int GAME;
int PLAYER_READY[maxclients];
int PLAYER_DEAD[maxclients];

struct instruction cmdparse(struct instruction i, char *in, int id); //David
void shake (struct instruction i, void *data, IPaddress *remoteIP, int id); //David
void dropclient(void *data, int id); //David
void broadcast(int *pool, struct client_t *clients, char *msg); //David
void tcpcast(int *pool, struct client_t *clients, char *msg); //David
void parse_move (char *string); //David
void parse_target (char *string); //David
void enough_players (struct client_t *clients, int *pool); //David
void ubroadcast(int *pool, struct client_t *clients, char *msg); //David
void printClients(int *pool, struct client_t *clients); //David
SDLNet_SocketSet create_sockset(TCPsocket server, UDPsocket usock, struct client_t *clients, int *client_pool); //David
int getslot(int *pool); //David
int cmdsend(int index, int x, int y, char *data, char *data2, TCPsocket sock); //David
void udptest (int *pool, struct client_t *clients, int id); //David
void mapsend(struct client_t *clients, int id); //David
void update_stats (int *client_pool, struct client_t *clients, char *string);  //David
void InitClients(struct client_t *cl); //David
void send_win(struct client_t *clients, int id); //David
int decide_winner(int *pool, struct client_t *clients); //David
