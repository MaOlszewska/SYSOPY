#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdbool.h>
#include <time.h>
#include <mqueue.h>
#include <string.h>

#include "common.h"
int IdClients[] = {0,0,0,0,0,0,0,0,0,0}; //zapisuje id klientów
key_t keysClients[10];
pid_t pidsClients[10];
int CurrentID = 1;
char types[5][5] = {"STOP", "LIST","ALL","ONE","INIT"};

mqd_t queue;
mqd_t clientQueue[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

void init_client(messagebuf * msg){
    // sprawdzam czy mogę dodać nowgo klienta do kolejki, max liczba ich to 10 bo po co komu więcej
    int i = 1;
    bool possible = false;
    while(i < 10){
        if(IdClients[i] == 0){
            possible = true;
            break;
        }
        i++;
    }
    if(possible == false){
        printf("Nie można dodać kolejnego klienta :( ");
        return ;
    }
    IdClients[i] = CurrentID;
    pidsClients[i] = msg->clientPid;
    char* name = calloc(256,sizeof(char));
    sprintf(name, "/%d", msg->clientPid);
    clientQueue[i] = mq_open(name, O_WRONLY);

    messagebuf message;
    message.messagetype = INIT;
    message.idClient = CurrentID;
    message.senderPid = getpid();

    IdClients[i] = CurrentID;

    printf("DODAŁEM KLIENTA o ID %d\n", CurrentID);
    if(mq_send(clientQueue[i], (const char *) &message, size, 0) != 0){
        printf("Nie udało się odesłać wiadomości :(");
        return ;
    }
    CurrentID ++;
}

void stop(int id){
    int i = 0;
    bool client = false;
    while(!client){
        if(IdClients[i] == id){
            client = true;
            break;
        }
        i++;
    }
    mq_close(clientQueue[i]);
    printf("Nie ma już klienta o id  %d \n", IdClients[i]);
    IdClients[i] = 0;
    keysClients[i] = 0;
    pidsClients[i] = 0;
    clientQueue[i] = -1;
}

void list_client(){
    printf("LISTA KLIENTÓW: \n");
    for(int i = 0; i < 10; i++){
        if(IdClients[i] != 0){
            printf("Client: %d\n", IdClients[i]);
        }
    }
}

void send_all(messagebuf * message){
    int current_client = message->idClient;

    for(int i = 0; i < 10; i++){
        if(IdClients[i] != 0 && IdClients[i] != current_client){
            printf("WYSŁAŁEM WIADOMOSĆ DO: %d \n", IdClients[i]);
            time_t t = time(NULL);
            struct tm *tm = localtime(&t);
            messagebuf new_message;
            sprintf(new_message.messagetext, "%s", message->messagetext);
            new_message.from_send = message-> idClient;
            new_message.date = tm;
            if(mq_send(clientQueue[i], (const char *) &new_message, size, 0) != 0){
                printf("Nie udało się odesłać wiadomości :(");
                return ;
            }
        }
    }
}

void send_one(messagebuf * message){
    int current_client = message->idClient;
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    messagebuf new_message;
    sprintf(new_message.messagetext, "%s", message->messagetext);
    new_message.from_send =current_client;
    new_message.date = tm;
    if(mq_send(clientQueue[message->to_send], (const char *) &new_message, size, 0) != 0){
        printf("Nie udało się odesłać wiadomości :(");
        return ;
    }
    printf("Wysłałem jedną wiadomosć");
}

void receive_message(messagebuf *message){
    FILE *ptr = fopen("test.txt","a");
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(ptr,"DATA: %d-%02d-%02d %02d:%02d:%02d %d %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, message->idClient, types[message->messagetype -1]);
    fclose(ptr);
    switch (message->messagetype){
        case INIT:  // dodaje nowego klienta
            init_client(message);
            break;
        case STOP: // klient konczy prace więc usuwam go
            stop(message->idClient);
            break;
        case ALL: // wysyłam komunikat do wsyztskich
            send_all(message);
            break;
        case ONE: // wysyłam komunikat do konkretnego klienta
            send_one(message);
            break;
        case LIST: // wypsiuje liste wszytskich aktywnych klientów
            list_client();
            break;
        default:
            printf("Mam problem z odczytaniem wiadomości");
    }
}

int main(int argc, char* argv[]) {

    struct mq_attr atrr;
    atrr.mq_maxmsg = 10;
    atrr.mq_msgsize = 256;

    queue = mq_open("/server_q", O_RDONLY | O_CREAT | O_EXCL, 0666, &atrr);

    if(queue == - 1){
        perror("Nie udało się stworzyć kolejki :(");
        exit(1);
    }

    messagebuf message;
    printf("TU SERWER :)\n");
    while(true){  //serwer sobie czeka na komunikaty
        // identyfikator kolejki, bufor do zapisu, rozmiar bufora,
        if(mq_receive(queue, (char *) &message, 256, NULL) == -1){
            perror("Nie udało się odebrać wiadomości :( ");
            exit(1);
        }
        receive_message(&message);
    }
}