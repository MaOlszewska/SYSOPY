#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

mode_t mode;
char* mode_name;
enum mode_t {KILL, SIGQUEUE, SIGRT};

int counter_signal = 0;
int number_signals;


void show_number(int sig, siginfo_t *info, void *ucontext){
    counter_signal += 1;
    if(mode == SIGQUEUE){
        printf("Tu Sender. Liczba sygnałów, które catcher mi wysłał to: %d\n", info->si_value.sival_int);
    }
}

void show_info(){
    printf("Tu znowu Sender, Otrzymałem właśnie sygnał SUGUSR2 \n");
    printf("Otrzymałem %d sygnałów, a powinienem otrzymać %d", counter_signal, number_signals);
    printf("Sender-Czas ze sobą skończyć...\n");
    exit(0);
}


int main(int argc, char* argv[]){

    int pid_catcher = atoi(argv[1]);
    number_signals = atoi(argv[2]);
    mode_name = argv[3];

    if(strcmp(mode_name, "KILL") == 0){mode = KILL;}
    else if(strcmp(mode_name, "SIGQUEUE") == 0){mode = SIGQUEUE;}
    else {mode = SIGRT;}

    struct sigaction action1;
    sigemptyset(&action1.sa_mask); // inicjalizuje zestaw sygnałów przekazywany przez set, mają być dodane do maski
    action1.sa_flags = SA_SIGINFO;
    action1.sa_sigaction = show_number; // co program ma zrobić po otzrymaniu sygnału
    if(mode == SIGRT){sigaction(SIGRTMIN, &action1, NULL);} // jeśli jest SIGRT to zmieniam na sygnał czasu rzeczywistego
    else{sigaction(SIGUSR1, &action1, NULL);}

    struct sigaction action2;
    sigemptyset(&action2.sa_mask); // inicjalizuje zestaw sygnałów przekazywany przez set, mają być dodane do maski
    action2.sa_flags = SA_SIGINFO;  // możlwiość wyswietlniea informacji o sygnale
    action2.sa_sigaction = show_info;  // zliczam ilosć sygnałów, które dotarły
    if(mode == SIGRT){sigaction(SIGRTMAX, &action2, NULL);}
    else{sigaction(SIGUSR1, &action2, NULL);}

    printf("Tu Sender.Teraz wyśle %d sygnałów SIGUSR1 do catchera \n", number_signals);
    for(int i = 1; i <= number_signals; i++){
       // printf("%d sygnał....\n", i);
        switch (mode) {
            case KILL:
                kill(pid_catcher, SIGUSR1);  // wysyłam sygnał do katchera
                break;
            case SIGQUEUE: {  //sygnał do catchera ale z dodatkową wartoscią
                union sigval value;
                value.sival_int += 1;
                sigqueue(pid_catcher, SIGUSR1, value);
                break;
            }
            case SIGRT:
                kill(pid_catcher, SIGRTMIN); // sygnałc czasu rzeczywistego
                break;
        }
    }

    printf("Tu znowu Sender. Właśnie skończyłem, jeszcze tylko wyśle SIGUSR2 \n");

    switch (mode) {
        case KILL:
            kill(pid_catcher, SIGUSR2);
            break;
        case SIGQUEUE: {  // to samo co kill ale jeszcze z dodatkową wartościa
            union sigval value;
            value.sival_int += 1;
            sigqueue(pid_catcher, SIGUSR2, value);
            break;
        }
        case SIGRT:
            kill(pid_catcher, SIGRTMAX); // wysłanie sygnału czasu rzeczywistego
            break;
    }
    while(1){}
}