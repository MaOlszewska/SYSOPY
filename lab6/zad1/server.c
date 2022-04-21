#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdbool.h>
#include <time.h>

#include "common.h"

int queue;
int IdClients[10] = {0,0,0,0,0,0,0,0,0,0}; //zapisuje id klientów
key_t keysClients[10];
pid_t pidsClients[10];
int CurrentID = 1;


void init_client(key_t key, pid_t pid){
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

//    int queueClient;
//    queueClient = msgget(key, IPC_CREAT); // tworzę nową kolejkę dla klienta
//    if(queueClient == - 1){
//        perror("Nie udało się stworzyć kolejki :(");
//        exit(1);
//    }

    // server odsyła identyfikator do klienta

    messagebuf message;
    message.messagetype = INIT;
    message.idClient = CurrentID;
    IdClients[i] = CurrentID;
    keysClients[i] = key;
    pidsClients[i] = pid;
    printf("%d ", key);
    printf("DODAŁEM KLIENTA o ID %d\n", CurrentID);
    CurrentID ++;
    if(msgsnd(key, &message, size, 0) != 0){
        printf("Nie udało się odesłać wiadomości :(");
        return ;
    }
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
    IdClients[i] = 0;
    keysClients[i] = 0;
    pidsClients[i] = 0;
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
            if(msgsnd(keysClients[i], &new_message, size, 0) != 0){
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
    if(msgsnd(keysClients[message->to_send], &new_message, size, 0) == -1){
        printf("Nie udało się odesłać wiadomości :(");
        return ;
    }
}

void receive_message(messagebuf *message){
    switch (message->messagetype){
        case INIT:  // dodaje nowego klienta
            init_client(message->clientKey, message-> clientPid);
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
    char * path = getenv("HOME");
    key_t key = ftok(path, 'A');
    if(key == -1){  // tworzę sobie unikalny klucz na podstawie ścieżki HOME dla kolejki
        perror("Nie udało się stworzyć unikalnego klucza :( ");
        exit(1);
    }

    queue = msgget(key, IPC_CREAT | 0666 ); //IPC_CREATE - tworzy kolejke w przypadku jej braku o takim kluczu
    printf("SERVER ID: %d", queue);
    if(queue == - 1){
        perror("Nie udało się stworzyć kolejki :(");
        exit(1);
    }
    messagebuf message;

    while(true){  //serwer sobie czeka na komunikaty
        // identyfikator kolejki, bufor do zapisu, rozmiar bufora,
        if(msgrcv(queue, &message, sizeof(message) - sizeof(message.messagetype), -INIT,0) == -1){
            perror("Nie udało się odebrać wiadomości :( ");
            exit(1);
        }
        receive_message(&message);
    }
}