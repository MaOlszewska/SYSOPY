#ifndef common
#define common
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <stdbool.h>


typedef struct messagebuf{
    long messagetype;  // typ komuniakatu
    char messagetext[256]; // treść komunikatu
    int idClient;
    key_t clientKey;
    pid_t clientPid;
    int to_send;
    int from_send;
    struct tm *date;
} messagebuf;

typedef enum type{
    STOP = 1, // najwyższy priorytet
    LIST = 2,
    ALL = 3,
    ONE = 4,
    INIT = 5
} type;

const int size = sizeof(messagebuf) - sizeof(long);

#endif