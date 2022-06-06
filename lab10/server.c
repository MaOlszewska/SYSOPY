#include "common.c"
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <poll.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BACKLOG 10

player* clients[CLIENTS];
game* games[CLIENTS / 2];

int TCP_port;
int local_socket;
int network_socket;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// zwracam indeks klienta o podanej nazwie
int clientIndex(char* name){
    for (int i = 0; i < CLIENTS; i++){
        if (clients[i] != NULL && strcmp(clients[i]->nick, name) == 0){return i;}
    }
    return -1;
}

// usuwam grę o podanym indeksie
void removeGame(int g_id){
    clients[games[g_id]->player1_id]->gameId = -1;
    clients[games[g_id]->player2_id]->gameId = -1;
    games[g_id] = NULL;
}
void checkGid(int g_id){
    if (g_id != -1){
        char* game_over_info = (char*) calloc(MESSAGE_LENGTH, sizeof(char));
        message* respond = createMessage("end", "server", game_over_info);
        free(game_over_info);
        free(respond->cmd);
        free(respond->message);
        free(respond->name);
        free(respond);
        removeGame(g_id);
    }
}

// usuwam klienta o podanej nazwie
void removeClient(char* name){
    int index = clientIndex(name);
    if (index != -1){
        int g_id = clients[index]->gameId;
        checkGid(g_id);
        free(clients[index]->nick);
        clients[index] = NULL;
    }
}

// dodaje gracza o podanej nazwie na pierwszy wolny indeks  i zwracam jego indeks
int addClient(char* name, int client_fd){
    int index = -1;
    // pierwszy wolny indeks
    for (int i = 0; i < CLIENTS; i++) if (clients[i] == NULL) index = i ;
    if (index != -1){
        // Tworzę nowego gracza
        player* player = calloc(1, sizeof(player));
        player->nick = (char*) calloc(1, NAME_LENGTH);
        player->gameId = -1;
        player->fd = -1;
        strcpy(player->nick, name);
        clients[index] = player;
        clients[index]->fd = client_fd;
    }
    return index;
}


int getMessage(){
    pthread_mutex_lock(&mutex);
    int clients_count = 0;
    for (int i = 0; i < CLIENTS; i++) if (clients[i] != NULL)clients_count ++;
    struct pollfd* fds = (struct pollfd*) calloc(2+clients_count, sizeof(struct pollfd));
    int socket_id = -1;

    fds[0].fd = local_socket;
    fds[0].events = POLLIN;
    fds[1].fd = network_socket;
    fds[1].events = POLLIN;
    int i = 2;
    for (int j = 0; j < CLIENTS && i < clients_count + 2; j++){
        if (clients[j] != NULL){
            fds[i].fd = clients[j]->fd;
            fds[i].events = POLLIN;
            i ++;
        }
    }
    pthread_mutex_unlock(&mutex);

    // funckja oczekuje na  zdarzenia i po ich wystąpienie aktualizuje zdarzenia na które oczekujemy
    poll(fds, clients_count + 2, -1);
    for (i = 0; i < clients_count + 2; i++){
        if (fds[i].revents & POLLIN)
        {
            socket_id = fds[i].fd;
            break;
        }
    }
    if (socket_id == local_socket || socket_id == network_socket){
        //akceptowanie oczekujących połączeń na gniazdach połączeniowych
        socket_id = accept(socket_id, NULL, NULL);
    }
    free(fds);
    return socket_id;
}
// wysyłam informacje o ruchu
void sendMoveInfo(int move_player_id, int wait_player_id, char move_player_sign, char wait_player_sign, char* board){
    message* respond_move_player;
    message* respond_wait_player;
    char* move_player_info = (char*) calloc(MESSAGE_LENGTH, sizeof(char));
    char* wait_player_info = (char*) calloc(MESSAGE_LENGTH, sizeof(char));
    sprintf(wait_player_info, "Jest ruch Twojego przeciwnika!\nprzeciwnik: %s\ntablica :\n%s\nznak: %c\n", clients[move_player_id]->nick, board, wait_player_sign);
    respond_wait_player = createMessage("opponent_move", "server", wait_player_info);
    sprintf(move_player_info, "Jest Twój ruch!\nprzeciwnik: %s\ntablica:\n%s\nznak: %c\n", clients[wait_player_id]->nick, board, move_player_sign);
    respond_move_player = createMessage("move", "server", move_player_info);

    // wysyłam wiadomości z użyciem gniazda
    send(clients[wait_player_id]->fd, messageToString(respond_wait_player), MESSAGE_STR_LENGTH, 0);
    send(clients[move_player_id]->fd, messageToString(respond_move_player), MESSAGE_STR_LENGTH, 0);
    free(respond_wait_player->cmd);
    free(respond_wait_player->message);
    free(respond_wait_player->name);
    free(respond_wait_player->cmd);
    free(respond_wait_player->message);
    free(respond_wait_player->name);
    free(respond_wait_player);
    free(respond_move_player);
    free(move_player_info);
    free(wait_player_info);

}

void sendEndInfo(int player1, int player2, int game_id, STATUS end_status){
    char res;
    switch (end_status){
        case O_WINNER:
            res = "O WYGRYWA!";
            break;
        case X_WINNER:
            res = "X WYGRYWA! ";
            break;
        case DRAW:
            res = "RYSUJ";
            break;
        default:
            res = "";
            break;
    }
    message* respond_player = createMessage("end", "server", res);
    removeGame(game_id);
    // wysyłam wiadomości z użyciem gniazda
    send(clients[player1]->fd, messageToString(respond_player), MESSAGE_STR_LENGTH, 0);
    send(clients[player2]->fd, messageToString(respond_player), MESSAGE_STR_LENGTH, 0);
    free(respond_player->cmd);
    free(respond_player->message);
    free(respond_player->name);
    free(respond_player);

}


//inicjalizacja Socektów
int initLocalSocket(char *socket_path){

    //funckja tworzy gniazdo i zwraca numer deskryptora
    int local_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    // struktura opisująca adres socetu, przechowuje informacje o podłączonym kliencie i określa gdzie ma nasłuchiwać
    struct sockaddr_un local_sockaddr;
    local_sockaddr.sun_family = AF_UNIX;
    strcpy(local_sockaddr.sun_path, socket_path);
    unlink(socket_path);
    //Nadaje socetowi jego nazwe żeby można przesyłać do niego dane
    if(bind(local_socket, (struct sockaddr*)&local_sockaddr, sizeof(struct sockaddr_un)) == -1){
        printf("BŁĘDZIK, upsik :( ");
        exit(1);}

    //odpowiada za rozpoczęcie akceptowania połączeń od klientów
    listen(local_socket, BACKLOG);
    return local_socket;

}

int initNetworkSocket(int port){
    // zawiera adresy powiązane z nazwą domenową
    struct hostent* server_entry = gethostbyname("localhost");
    struct in_addr server_address = *(struct in_addr*) server_entry->h_addr;

    struct sockaddr_in network_sockaddr;
    network_sockaddr.sin_family = AF_INET;// adress family
    network_sockaddr.sin_port = htons(port); // port adress
    network_sockaddr.sin_addr.s_addr = server_address.s_addr; // internet adrees

    //funckja tworzy gniazdo i zwraca numer deskryptora
    int network_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (network_socket == -1){
        printf("BŁĘDZIK, smuteczek :( ");
        exit(1);}

    //Nadaje socetowi jego nazwe żeby można przesyłać do niego dane
    if(bind(network_socket, (struct sockaddr*) &network_sockaddr, sizeof(network_sockaddr)) == -1){
        printf("Coś poszło nie tak :( ");
        exit(1);}

    //odpowiada za rozpoczęcie akceptowania połączeń od klientów
    if(listen(network_socket, BACKLOG) == -1){
        printf("Nie mogę już słuchać :( ");
        exit(1);}
    printf("SIEĆ SERVERA: %s:%d\n", inet_ntoa(server_address), port);
    return network_socket;

}

int main(int argc , char** argv){

    if(argc != 3){
        printf("ZŁA LICZBA ARGUMNETÓW! :( ");
        exit(1);
    }
    // server ptzyjmuje numer portu TCP-strumieniowy i scieżke gniazda UNIX
    TCP_port = atoi(argv[1]);
    char* socket_path = argv[2];

    // tworzenie gzniazda lokalnego i sieciowego
    local_socket = initLocalSocket(socket_path);
    network_socket = initNetworkSocket(TCP_port);

    while(1){
        //pobiera wiadomość
        int client_fd = getMessage();
        char buf[MESSAGE_STR_LENGTH];
        read(client_fd, buf, MESSAGE_STR_LENGTH);
        message* msg = parseMessage(buf);

        pthread_mutex_lock(&mutex);
        // dodaje nowego gracza
        if (strcmp(msg->cmd, "add") == 0){
            // informuje, że nazwa jest zajęta
            if (clientIndex(msg->name) != -1){
                message* respond = createMessage("add", "server", "nick is already taken");
                send(client_fd, messageToString(respond), sizeof(message), 0);
                close(client_fd);
                free(respond->cmd);
                free(respond->message);
                free(respond->name);
                free(respond);
            }
            else{
                int enemy_id = -1;
                // tworzę nowego gracza
                for (int i = 0; i < CLIENTS; i++){
                    if (clients[i] != NULL && clients[i]->gameId == -1){enemy_id = i;}
                }

                int given_index = addClient(msg->name, client_fd);
                if (enemy_id == -1){
                    //powiadamiwam o zbliżającej się grze
                    message* respond = createMessage("add", "server", "waiting for enemy");
                    send(client_fd, messageToString(respond), MESSAGE_STR_LENGTH, 0);
                    free(respond->cmd);
                    free(respond->message);
                    free(respond->name);
                    free(respond);
                }
                else{
                    //jeśli jest możliwe dodanie nowej gry to tworze ją
                    int free_game_id = -1;
                    // pierwszy wolny indeks gry nowej
                    for (int i = 0; i < CLIENTS / 2; i++){
                        if (games[i] == NULL) free_game_id = i;}
                    games[free_game_id] = createGamee(given_index, enemy_id);
                    game* g = games[free_game_id];
                    char* game_init_info = (char*) calloc(MESSAGE_LENGTH, sizeof(char));
                    // powiadamiam obydwu graczy o grze, przeciwniku i znaku jakim grają
                    sprintf(game_init_info, "GRA SIĘ ZACZĘŁA !\nprzeciwnik: %s\nplansza:\n%s\nznak: %c\n", clients[enemy_id]->nick,
                            boardToString(g->board),
                            signToChar(g->player1_sign));
                    message* respond = createMessage("start", "server", game_init_info);
                    send(client_fd, messageToString(respond), MESSAGE_STR_LENGTH, 0);
                    memset(game_init_info, 0, strlen(game_init_info));
                    sprintf(game_init_info, "GRA SIĘ ZACZĘŁA !\nprzeciwnik: %s\nplansza:\n%s\nznak: %c\n", clients[given_index]->nick,
                            boardToString(g->board),
                            signToChar(g->player2_sign));
                    free(respond->cmd);
                    free(respond->message);
                    free(respond->name);

                    respond = createMessage("start", "server", game_init_info);
                    send(clients[enemy_id]->fd, messageToString(respond), MESSAGE_STR_LENGTH, 0);
                    clients[enemy_id]->gameId = free_game_id;
                    clients[given_index]->gameId = free_game_id;
                    free(respond->cmd);
                    free(respond->message);
                    free(respond->name);
                    free(game_init_info);
                    free(respond);

                    sendMoveInfo(g->player1_id, g->player2_id, signToChar(g->player1_sign),
                                 signToChar(g->player2_sign), boardToString(g->board));
                }
            }
        }
        // jesli gracz wyszedł to go usuwam
        else if(strcmp(msg->cmd, "quit") == 0){
            removeClient(msg->name);
        }
        // jeśli graczwykonał ruch
        else if(strcmp(msg->cmd, "move") == 0){
            int moved_index = clientIndex(msg->name);
            int g_id = clients[moved_index]->gameId;
            int opponent_index;
            int player_id;
            games[g_id]->player1_id == moved_index ? (player_id = 1) : (player_id = 2);
            int place = atoi(msg->message);
            SIGN sign;
            SIGN opponent_sign;

            // rysuje znak na planszy z grą
            if (games[g_id]->board[place] == FREE){
                if (player_id == 1){
                    sign = games[g_id]->player1_sign;
                    opponent_sign = games[g_id]->player2_sign;
                }
                else{
                    sign = games[g_id]->player2_sign;
                    opponent_sign = games[g_id]->player1_sign;
                }
                games[g_id]->board[place] = sign;
            }

            STATUS gs = getGameStatus(*games[g_id]);

            switch (gs){
                case RUNNING:
                    sendMoveInfo(opponent_index, moved_index, signToChar(opponent_sign), signToChar(sign),
                                 boardToString(games[g_id]->board));
                    break;
                default:
                    sendEndInfo(moved_index, opponent_index, g_id, gs);
            }

        }
        free(msg->cmd);
        free(msg->message);
        free(msg->name);
        free(msg);
        pthread_mutex_unlock(&mutex);
    }
    return 0;

}