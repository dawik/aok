/*
File: net.c
By: David Wikmans
Created 21 april, 2011.
*/

#define MIN_STR_LEN 6

/* Koder för olika instruktioner - David*/
enum protocol { 
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

/* Struktur för att enklare hantera en instruktionsträngs olika segment -- David*/
struct instruction { 
        int id;
        char data[6][128];
};

/* Nät-trådar */
int netThread(void *data); //David
int udpThread(void *data); //David

/* Skickar en godtycklig instruktion till server */
int cmdsend(int index, int x, int y, char *data, char *data2); //David
/* Parsear en hälso sträng och uppdatera klienten med detta -- David */
void UpdateHealth(char* string);
/* Parsear en resurs sträng och uppdatera klienten med detta -- David*/
void UpdateResources(char* string); 
