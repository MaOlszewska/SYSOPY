#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

mode_t mode;
char* mode_name;
enum mode_t {IGNORE, HANDLER, MASK, PENDING};

void handler(int signal){
    printf("Otrzymałem sygnał %d \n", signal);
}

void ifBlocked(){
    sigset_t blocked;
    sigpending(&blocked);  // Służy do odczytania listy sygnałów, które oczekuję na odblokowanie w danym procesie
    if(sigismember(&blocked, SIGUSR1) == 1){ // czy sygnał SIGUSR1 należy do blocked jeśli tak to zwraca 1
        printf("TAK \n");}
    else{printf("NIE \n");}
}

int main(int argc, char* argv[]) {

    if(argc != 2) {
        printf("Przykro mi, nie mam argumnetu :( ");
        return -1;
    }
    mode_name = argv[1];

    if(strcmp(mode_name, "ignore") == 0){mode = IGNORE;}
    else if(strcmp(mode_name, "mask") == 0){mode = MASK;}
    else if(strcmp(mode_name, "pending") == 0){mode = PENDING;}
    else {mode = HANDLER;}

    // SIGUSR1 wysyłany do procesu w celu wskazania warunków zdefiniowanych przez użytkownika
    switch (mode) {
        case IGNORE:{
            printf("ARGUMNET IGNORE \n");
            signal(SIGUSR1, SIG_IGN);  // SIG_IGN- sygnał jest ignorowany  signal- funckja obsługi sygnału
            printf("Przed wygenerowaniem rodzica \n");
            raise(SIGUSR1);  // generuje sygnał w bieżacym procesie sig- numer sygnału do wysłania
            printf("Po wygenerowaniu rodzica \n");

            pid_t pid = fork(); // tworzy się nowy proces
            if (pid == 0){
                printf("Przed wygenerowaniem dziecka\n");
                raise(SIGUSR1);  // potomek generuje sygnał dla samego siebie
                printf("Po wygenerowaniu dziecka \n");
            }
            else {wait(NULL);}
            break;
        }
        case HANDLER:{
            printf("ARGUMENT HANDLER \n");
            signal(SIGUSR1, handler);  // w handler- przekazujemy co proces ma zrobić kiedy otrzyma sygnał sigusr1
            printf("Przed wygenerowaniem rodzica \n");
            raise(SIGUSR1);  // generuje sygnał sig- numer sygnału do wysłania
            printf("Po wygenerowaniu rodzica \n");

            pid_t pid = fork(); // tworzy się nowy proces
            if (pid == 0){
                printf("Przed wygenerowaniem dziecka\n");
                raise(SIGUSR1);  // potomek generuje sygnał dla samego siebie
                printf("Po wygenerowaniu dziecka \n");
            }
            else {wait(NULL);}
            break;
        }
        case MASK:{
            printf("ARGUMENT MASK\n");
            sigset_t set_mask; // typ służący do reprezentowania zestawu sygnałów
            sigemptyset(&set_mask);  // inicjuje zestaw sygnałów wskazywany przez set_mask
            sigaddset(&set_mask, SIGUSR1);  // dodaje syganł do zestawu sygnału które mają być blokowane

            if(sigprocmask(SIG_SETMASK, &set_mask, NULL) != 0){    // ustawia maske na sygnał wskazywany przez set_mask
                printf("Błąd \n");
                return 1;
            }
            printf("Ustawienie maski powiodło się \n");

            raise(SIGUSR1);  // proces sam sobie wysyła sygnał SIGUSR1
            printf("Czy sygnał rodzica jest widoczny dla niego? \n");
            ifBlocked();

            pid_t pid = fork(); // tworzy się nowy proces
            if (pid == 0){
                if(mode == MASK) {
                    printf("Czy sygnał dziecka jest widoczny w dziecku?\n");
                    raise(SIGUSR1); // potomek wysyła sygnał do samego siebie
                    ifBlocked();
                }
                printf("Czy sygnał rodzica jest widoczny w dziecku? \n"); // czy sygnał w przodku jest widoczny w potomku
                ifBlocked();
            }
            else {wait(NULL);}
        }
        case PENDING:{
            printf("ARGUMENT PENDING\n");
            sigset_t set_mask; // typ służący do reprezentowania zestawu sygnałów
            sigemptyset(&set_mask);  // inicjuje zestaw sygnałów wskazywany przez set_mask
            sigaddset(&set_mask, SIGUSR1);  // dodaje sygnał do setu sygnałów które mają być blokowane

            if(sigprocmask(SIG_SETMASK, &set_mask, NULL) != 0){    // ustawia maske na sygnały wskazywany przez set_mask
                printf("Błąd \n");
                return 1;
            }
            printf("Ustawienie maski powiodło się \n");

            raise(SIGUSR1);  // wysyła sygnał sam do siebie
            printf("Czy sygnał rodzica jest widoczny dla niego? \n");
            ifBlocked();

            pid_t pid = fork(); // tworzy się nowy proces
            if (pid == 0){
                printf("Czy sygnał rodzica jest widoczny w dziecku? \n"); // czy sygnał w przodku jest widoczny w potomku
                ifBlocked();
            }
            else {wait(NULL);}
        }
    }
    return 0;
}

