#include "common.c"
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <string.h>
#include <poll.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>

char* name;
int client_socket;
STATUS status;
TURN turn;
pthread_t game_thread;

pthread_mutex_t variable_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t variable_cond = PTHREAD_COND_INITIALIZER;

void quit(){
    message* msg = createMessage("quit", name, "");
    char* string = messageToString(msg);
    send(client_socket, string, MESSAGE_STR_LENGTH, 0);
    free(msg->cmd);
    free(msg->message);
    free(msg->name);
    free(msg);
    exit(0);
}

void initServerConnectionLocal(char* server_path){

    //funckja tworzy gniazdo i zwraca numer deskryptora
    client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_socket == -1){
        printf("BŁĘDZIK :(");
        exit(1);}
    struct sockaddr_un local_sockaddr;
    local_sockaddr.sun_family = AF_UNIX;
    strcpy(local_sockaddr.sun_path, server_path);
    // łacze klienta z serverem
    if(connect(client_socket, (struct sockaddr *)&local_sockaddr, sizeof(struct sockaddr_un)) != 0){
        printf("MAMY MAŁY PROBLEMIK \n");
        exit(1);
    }

    message* msg = createMessage("add", name, "");
    char* string = messageToString(msg);
    send(client_socket, string, MESSAGE_STR_LENGTH, 0);
    free(msg->cmd);
    free(msg->message);
    free(msg->name);
    free(msg);
}
//łącze server przez sieć
void initServerConnectionNetwork(int port, char* server_address){
    struct sockaddr_in server_sock;

    server_sock.sin_family = AF_INET;
    server_sock.sin_port = htons(port);
    server_sock.sin_addr.s_addr = inet_addr(server_address);
    //funckja tworzy gniazdo i zwraca numer deskryptora
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        printf("Upsik, coś jest źle :( ");
        exit(1);
    }
    //łacze gracza z serwerem
    connect(client_socket, (struct sockaddr*) &server_sock, sizeof(server_sock));

    message* msg = createMessage("add", name, "");
    char* string = messageToString(msg);
    send(client_socket, string, MESSAGE_STR_LENGTH, 0);
    free(msg->cmd);
    free(msg->message);
    free(msg->name);
    free(msg);

}

void getMessage(){
    struct pollfd pfd;

    pfd.fd = client_socket;
    pfd.events = POLLIN;

    poll(&pfd, 1, -1);

}

void* playGame(){
    pthread_setcancelstate(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    while(status == RUNNING){
        pthread_mutex_lock(&variable_mutex);
        while (turn == PLAYER_2){
            pthread_cond_wait(&variable_cond, &variable_mutex);
        }
        pthread_mutex_unlock(&variable_mutex);
        char* input = (char*) calloc(2, sizeof(char));
        // pobieram nazwę od gracza
        scanf("%s", input);
        pthread_mutex_lock(&variable_mutex);

        message* msg = createMessage("move", name, input);
        char* string = messageToString(msg);
        // wysyłam dane do gniazda
        send(client_socket, string, MESSAGE_STR_LENGTH, 0);
        free(msg->cmd);
        free(msg->message);
        free(msg->name);
        free(input);
        free(msg);
        turn = PLAYER_2;

        pthread_mutex_unlock(&variable_mutex);

    }
    return NULL;
}

int main(int argc, char** argv){
    if (argc != 4 && argc != 5) {
        printf("Zła liczba argumnetów");
        exit(1);
    }
    // Pobieram i zapisuje Nazwę gracza, sposób połaczenia z serwerem i adres serwera
    name = argv[1];
    char* service = argv[3];
    int server_port;
    status = END;
    turn = PLAYER_2;

    signal(SIGINT, quit);

    // W zalezności jaki sposób połączenia, inicjuje server Lokalnie lub Przez sieć
    if (strcmp(argv[2], "local") == 0) initServerConnectionLocal(service);
    else if (strcmp(argv[2], "network") == 0){
        server_port = atoi(argv[4]);
        initServerConnectionNetwork(server_port, service);
    }
    else{
        printf("Problemik :(");
        exit(1);}

    while(1){
        getMessage();
        message* msg;
        char buf[MESSAGE_STR_LENGTH];
        recv(client_socket, buf, MESSAGE_STR_LENGTH, 0);
        msg = parseMessage(buf);
        if (strcmp(msg->cmd, "add") == 0){ printf("%s\n",msg->message); }
        else if (strcmp(msg->cmd, "start") == 0){
            printf("%s", msg->message);
            pthread_mutex_lock(&variable_mutex);
            status = RUNNING;
            pthread_create(&game_thread, NULL, &playGame, NULL);
            pthread_mutex_unlock(&variable_mutex);

        }
        else if (strcmp(msg->cmd, "end") == 0){
            printf("%s\n", msg->message);
            pthread_cancel(game_thread);
            status = END;
            turn = PLAYER_2;
            quit();
        }
        else if (strcmp(msg->cmd, "move") == 0){
            printf("%s", msg->message);
            pthread_mutex_lock(&variable_mutex);
            turn = PLAYER_1;
            pthread_cond_signal(&variable_cond);
            pthread_mutex_unlock(&variable_mutex);

        }
        else if (strcmp(msg->cmd, "opponent_move") == 0){ printf("%s", msg->message);}
        free(msg->cmd);
        free(msg->message);
        free(msg->name);
        free(msg);
    }
    return 0;
}