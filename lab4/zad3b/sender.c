#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

char* mode;
int counter_signal = 0;
int number_signals;
bool wait;

void handler_queue(int sig, siginfo_t *info, void *ucontext){
    counter_signal += 1;
    wait = false;
    if(strcmp(mode, "SIGQUEUE") == 0){
        // printf("Tu Sender. Liczba sygnałów, które catcher mi wysłał to: %d\n", info->si_value.sival_int);
    }
}

void handler2(){
    printf("Tu znowu Sender, Otrzymałem właśnie sygnał SUGUSR2 \n");
    printf("Otrzymałem %d sygnałów, a powinienem otrzymać %d", counter_signal, number_signals);
    printf("Sender-Czas ze sobą skończyć...\n");
    exit(0);
}


int main(int argc, char* argv[]){

    int pid_catcher = atoi(argv[1]);
    number_signals = atoi(argv[2]);
    mode = argv[3];

    wait = false;

    struct sigaction action1;
    sigemptyset(&action1.sa_mask);
    action1.sa_flags = SA_SIGINFO;
    action1.sa_sigaction = handler_queue;
    if(strcmp(mode, "SIGRT") == 0){
        sigaction(SIGRTMIN, &action1, NULL);
    }
    else{
        sigaction(SIGUSR1, &action1, NULL);
    }


    struct sigaction action2;
    sigemptyset(&action2.sa_mask); // inicjalizuje zestaw sygnałów przekazywany przez set, mają być dodane do maski
    action2.sa_flags = SA_SIGINFO;
    action2.sa_sigaction = handler2;  // zliczam ilosć sygnałów, które dotarły
    if(strcmp(mode, "SIGRT") == 0){
        sigaction(SIGRTMAX, &action2, NULL);
    }
    else{
        sigaction(SIGUSR2, &action2, NULL);
    }

    printf("Tu Sender.Teraz wyśle %d sygnałów SIGUSR1 do catchera \n", number_signals);
    for(int i = 1; i <= number_signals; i++){

       while(wait){}
       wait = true;
       if(strcmp(mode, "KILL") == 0){
           kill(pid_catcher, SIGUSR1);   // wysyłam sygnały  spowrotem za pomocą komendy kill
       }
       else if(strcmp(mode, "SIGQUEUE") == 0){
           union sigval value;
           value.sival_int += 1;
           sigqueue(pid_catcher,SIGUSR1, value);  // value - numer kolejnego odsyłanego sygnału
       }
       else {
           kill(pid_catcher, SIGRTMIN );
       }
    }
    while(wait){}

    printf("Tu znowu Sender. Właśnie skończyłem, jeszcze tylko wyśle SIGUSR2 \n");
    if(strcmp(mode, "KILL") == 0){
        kill(pid_catcher, SIGUSR2);   // wysyłam sygnały  spowrotem za pomocą komendy kill
    }
    else if(strcmp(mode, "SIGQUEUE") == 0){
        union sigval value;
        sigqueue(pid_catcher,SIGUSR2, value);  // value - numer kolejnego odsyłanego sygnału
    }
    else {
        kill(pid_catcher, SIGRTMAX);
    }
    while(1){}
}