#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdbool.h>
#include <time.h>

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
};

const int size = sizeof(messagebuf) - sizeof(long);

int queue;
int IdClients[10]; //zapisuje id klientów
key_t keysClients[10];
pid_t pidsClients[10];
int CurrentID = 0;


void init_client(key_t key, pid_t pid){
    // sprawdzam czy mogę dodać nowgo klienta do kolejki, max liczba ich to 10 bo po co komu więcej
    int i = 0;
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

    int queueClient;
    queueClient = msgget(key, IPC_CREAT); // tworzę nową kolejkę dla klienta
    if(queueClient == - 1){
        perror("Nie udało się stworzyć kolejki :(");
        exit(1);
    }

    // server odsyła identyfikator do klienta

    messagebuf message;
    message.messagetype = INIT;
    message.idClient = CurrentID;
    IdClients[i] = CurrentID;
    keysClients[i] = key;
    pidsClients[i] = pid;
    if(msgsnd(queueClient, &message, size, 0) == -1){
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
            time_t t = time(NULL);
            struct tm *tm = localtime(&t);
            messagebuf new_message;
            sprintf(new_message.messagetext, "%s", message->messagetext);
            new_message.from_send = message-> idClient;
            new_message.date = tm;
            if(msgsnd(keysClients[i], &new_message, size, 0) == -1){
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
            init_client(message-> idClient, message-> clientPid);
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
    key_t key = ftok(path, "C");
    if(key == -1){  // tworzę sobie unikalny klucz na podstawie ścieżki HOME dla kolejki
        perror("Nie udało się stworzyć unikalnego klucza :( ");
        exit(1);
    }

    queue = msgget(key, IPC_CREAT); //IPC_CREATE - tworzy kolejke w przypadku jej braku o takim kluczu

    if(queue == - 1){
        perror("Nie udało się stworzyć kolejki :(");
        exit(1);
    }

    messagebuf message;

    while(true){  //serwer sobie czeka na komunikaty
        // identyfikator kolejki, bufor do zapisu, rozmiar bufora,
        if(msgrcv(queue, &message, size, -INIT,0)){
            perror("Nie udało się odebrać wiadomości :( ");
            exit(1);
        }
        receive_message(&message);
    }
}