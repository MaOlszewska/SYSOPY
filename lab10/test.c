#include "common.c"

int main(int argc, char** argv){
     game* g = createGamee(0, 1);

     g->board[3] = X;
     char gm[MESSAGE_LENGTH];
     sprintf(gm, "BOB:%s:%c", boardToString(g->board), signToChar(X));

     gameMessage* msg = (gameMessage*) calloc(1, sizeof(gameMessage));
     msg->opponent = (char*) calloc(1, NAME_LENGTH);
     msg->board = (char*) calloc(1, MESSAGE_LENGTH);
     char* start; char* end;
     start= strtok_r(gm, ":", &end);
     strcpy(msg->opponent, start);
     start = strtok_r(NULL, ":", &end);
     strcpy(msg->board, start);
     msg->sign = end[0];

     printf("IMIE: %s\nPLANSZA: %s\nZNAK: %c\n",msg->opponent, msg->board, msg->sign);
     return 0;
}
