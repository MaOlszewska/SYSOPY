#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

int count_signal = 0;
mode_t mode;
pid_t pid_sender;
char* mode_name;
enum mode_t {KILL, SIGQUEUE, SIGRT};

void send_back(int sig, siginfo_t *info, void *ucontext){
    pid_sender = info->si_pid;
    count_signal += 1; // jesli syganł jest odebrany to wysyłam potwierdzenie
    switch (mode) {
        case KILL:
            kill(pid_sender, SIGUSR1);
            break;
        case SIGQUEUE: {
            union sigval value;
            //value.sival_int += 1;
            sigqueue(pid_sender, SIGUSR1, value);
            break;
        }
        case SIGRT:
            kill(pid_sender, SIGRTMIN);
            break;
    }
}

void send_last(int sig, siginfo_t *info, void *ucontext){
    pid_sender = info -> si_pid;

    printf("Tu znowu catcher, odebrałem %d sygnałów, teraz wyśle je spowrotem \n", count_signal);

    // kiedy wyśle wszytskie SIGUSR1 wysyłam jeden sygnał SIGUSR2 i wyspiuje liczbe odebranych sygnałów i koncze działanie :)
    printf("Jeszce wyśle SIGUSR2\n");
    switch (mode) {
        case KILL:
            kill(pid_sender, SIGUSR2);
            break;
        case SIGQUEUE: {
            union sigval value;
            //value.sival_int += 1;
            sigqueue(pid_sender, SIGUSR2, value);
            break;
        }
        case SIGRT:
            kill(pid_sender, SIGRTMAX);
            break;
    }
    printf(" Catcher - Czas ze sobą skończyć...\n");
    exit(0);
}

int main(int argc, char* argv[]){

    printf("\n Jestem catcher, a tu mój numer PID: %d\n", getpid());
    mode_name = argv[1];

    if(strcmp(mode_name, "KILL") == 0){mode = KILL;}
    else if(strcmp(mode_name, "SIGQUEUE") == 0){mode = SIGQUEUE;}
    else {mode = SIGRT;}

    printf("Czekam na sygnały...... \n");

    struct sigaction action1;
    sigemptyset(&action1.sa_mask); // inicjalizuje zestaw sygnałów przekazywany przez set, mają być dodane do maski
    action1.sa_sigaction = send_back;  // zliczam ilosć sygnałów, które dotarły
    action1.sa_flags = SA_SIGINFO;
    if(mode == SIGRT){
        sigaction(SIGRTMIN, &action1, NULL);
    }
    else{
        sigaction(SIGUSR2, &action1, NULL);
    }

    struct sigaction action2;
    sigemptyset(&action2.sa_mask); // inicjalizuje zestaw sygnałów przekazywany przez set, mają być dodane do maski
    action2.sa_flags = SA_SIGINFO;
    action2.sa_sigaction = send_last;
    if(mode == SIGRT){
        sigaction(SIGRTMAX, &action2, NULL);
    }
    else{
        sigaction(SIGUSR2, &action2, NULL);
    }

    while(1){}
}