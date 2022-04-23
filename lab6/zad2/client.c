#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include "common.h"

mqd_t queue_s;
mqd_t queue_c;
int current_ID;
pid_t pid;

void init(){
    messagebuf message;
    message.messagetype = INIT;
    message.clientPid = pid;

    if(mq_send(queue_s, (const char *) &message, size, 0) != 0){
        printf("Nie udało się odesłać wiadomości :(");
        return ;
    }
    messagebuf receive_message;
    if(mq_receive(queue_s, (char *) &receive_message, 256, NULL) == -1){
        perror("Nie udało się odebrać wiadomości :( ");
        exit(1);
    }
    current_ID = receive_message.idClient;
}

void stop(){
    messagebuf message;
    message.messagetype = STOP;
    message.idClient = current_ID;
    if(mq_send(queue_s, (const char *) &message, size, 0) != 0){
        printf("Nie udało się odesłać wiadomości :(");
        return ;
    }
    else{
        char* name = calloc(256,sizeof(char));
        sprintf(name, "/%d", pid);
        printf("Znikam\n");
        mq_close(queue_s);
        mq_close(queue_s);
        mq_unlink(name);
        exit(0);
    }
}
void list(){
    messagebuf message;
    message.idClient = current_ID;
    message.messagetype = LIST;
    if(mq_send(queue_s, (const char *) &message, size, 0) != 0){
        printf("Nie udało się odesłać wiadomości :(");
        return ;
    }
    printf("CHCE ŻEBYŚ MI POKAZAŁ KLIENTÓW \n");
}


int main(int argc, char* argv[]) {

    queue_s = mq_open("/server_q", O_WRONLY);

    struct mq_attr atrr;
    atrr.mq_maxmsg = 10;
    atrr.mq_msgsize = 256;
    pid = getpid();
    char* name = calloc(256,sizeof(char));
    sprintf(name, "/%d", pid);
    queue_c = mq_open(name, O_CREAT | O_RDWR | O_EXCL , 0666, &atrr);
    init();
    char line[256];
    char buff[256];
    int tmpid;
    while(true){
        fgets(line, 256, stdin);
        line[strcspn(line, "\n")] = '\0';

        if(strcmp(line, "STOP") == 0){
            stop();
        }
        else if(strcmp(line, "LIST") == 0){
            list();
        }
        else if(strcmp(line, "ALL") == 0){
            time_t t;
            time(&t);
            messagebuf message;
            message.messagetype = ALL;
            message.idClient = current_ID;
            message.date = t;
            printf("PODAJ TREŚĆ WIADOMOŚCI: \n");
            scanf("%c ", buff);
            strcpy(message.messagetext, buff);
            if(mq_send(queue_s, (const char *) &message, size, 0) != 0){
                printf("Nie udało się odesłać wiadomości :(");
                exit(1);
            }
            printf("WYSŁAŁEM WIADOMOŚĆ DO WSZYSTKICH \n");
        }
        else if(strcmp(line, "ONE") == 0){
            time_t t;
            time(&t);
            printf("PODAJ TREŚĆ WIADOMOŚCI: \n");
            scanf("%c ", buff);
            printf("PODAJ NUMER ID KLIENTA \n");
            scanf("%s ", buff);
            tmpid = atoi(buff);
            messagebuf message;
            message.messagetype = ONE;
            message.idClient = current_ID;
            message.to_send = tmpid;
            message.date = t;
            strcpy(message.messagetext, buff);
            if(mq_send(queue_s, (const char *) &message, size, 0) != 0){
                printf("Nie udało się odesłać wiadomości :(");
                exit(1);
            }
        }
        else{
            printf("Otrzymałem wiadomość \n");
        }
    }

}