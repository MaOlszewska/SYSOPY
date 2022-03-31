#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
int count_signal = 0;
char* mode;

void handler1(){
    count_signal += 1;
}

void handler2(int sig, siginfo_t *info, void *ucontext){
    pid_t pid_sender = info -> si_pid;

    printf("Tu znowu catcher, odebrałem %d sygnałów, teraz wyśle je spowrotem \n", count_signal);
    for(int i = 1; i <= count_signal; i++){  //wysyłam tyle sygnałów  1 ile otrzymano 1
        if(strcmp(mode, "KILL") == 0){
            kill(pid_sender, SIGUSR1);   // wysyłam sygnały  spowrotem za pomocą komendy kill
        }
        else if(strcmp(mode, "SIGQUEUE") == 0){
            union sigval value;
            value.sival_int = i;
            sigqueue(pid_sender,SIGUSR1, value);  // value - numer kolejnego odsyłanego sygnału
        }
        else {
            kill(pid_sender, SIGRTMIN);
        }
    }
    // kiedy wyśle wszytskie SIGUSR1 wysyłam jeden sygnał SIGUSR2 i wyspiuje liczbe odebranych sygnałów i koncze działanie :)
    printf("Jeszce wyśle SIGUSR2\n");
    if(strcmp(mode, "KILL") == 0){
        kill(pid_sender, SIGUSR2);   // wysyłam sygnały  spowrotem za pomocą komendy kill
    }
    else if(strcmp(mode, "SIGQUEUE") == 0){
        union sigval value;
        sigqueue(pid_sender,SIGUSR2, value);  // value - numer kolejnego odsyłanego sygnału
    }
    else {
        kill(pid_sender, SIGRTMAX);
    }


    printf(" Catcher - Czas ze sobą skończyć...\n");
    exit(0);
}

int main(int argc, char* argv[]){

    printf("\n Jestem catcher, a tu mój numer PID: %d\n", getpid());
    mode = argv[1];

    printf("Czekam na sygnały...... \n");

    struct sigaction action1;
    sigemptyset(&action1.sa_mask); // inicjalizuje zestaw sygnałów przekazywany przez set, mają być dodane do maski
    action1.sa_sigaction = handler1;  // zliczam ilosć sygnałów, które dotarły
    if(strcmp(mode, "SIGRT") == 0){
        sigaction(SIGRTMIN, &action1, NULL);
    }
    else{
        sigaction(SIGUSR1, &action1, NULL);
    }

    struct sigaction action2;
    sigemptyset(&action2.sa_mask); // inicjalizuje zestaw sygnałów przekazywany przez set, mają być dodane do maski
    action2.sa_flags = SA_SIGINFO;
    action2.sa_sigaction = handler2;
    if(strcmp(mode, "SIGRT") == 0){
        sigaction(SIGRTMAX, &action2, NULL);
    }
    else{
        sigaction(SIGUSR2, &action2, NULL);
    }

    while(1){}
}