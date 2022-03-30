#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

void handler(int signal){
    printf("Otrzymałem sygnał %d \n", signal);
}

void ifBlocked(){
    sigset_t blocked;
    sigpending(&blocked);  // sprawdza oczekujace sygnały
    if(sigismember(&blocked, SIGUSR1) == 1){ // czy sygnał SIGUSR1 należy do blocked jeśli tak to zwraca 1
        printf("TAK \n");}
    else{printf("NIE \n");}
}


int main(int argc, char* argv[]) {

    if(argc != 2) {
        printf("Przykro mi, nie mam argumnetu :( ");
        return -1;
    }
    // SIGUSR1 wysyłany do procesu w celu wskazania warunków zdefiniowanych przez użytkownika
    if(strcmp(argv[1], "ignore") == 0){
        printf("ARGUMNET IGNORE \n");
        signal(SIGUSR1, SIG_IGN);  // SIG_IGN- sygnał jest ignorowany
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
    }


    else if(strcmp(argv[1], "handler") == 0){
        printf("ARGUMENT HANDLER \n");
        signal(SIGUSR1, handler);
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
    }


    else if(strcmp(argv[1], "mask") == 0 || strcmp(argv[1], "pending") == 0 ){
        if(strcmp(argv[1], "mask") == 0) { printf("ARGUMENT MASK\n");}
        else{ printf("ARGUMENT PENDING\n");}

        sigset_t set_mask; // typ służący do reprezentowania zestawu sygnałów
        sigemptyset(&set_mask);  // inicjuje zestaw sygnałów wskazywany przez set_mask
        sigaddset(&set_mask, SIGUSR1);  // maskuje sygnał

        if(sigprocmask(SIG_SETMASK, &set_mask, NULL) != 0){    // ustawia maske na sygnał wskazywany przez set_mask
            printf("Błąd \n");
            return 1;
        }
        printf("Ustawienie maski powiodło się \n");

        raise(SIGUSR1);
        printf("Czy sygnał rodzica jest widoczny dla niego? \n");
        ifBlocked();

        pid_t pid = fork(); // tworzy się nowy proces
        if (pid == 0){
            if(strcmp(argv[1], "mask") == 0) {
                printf("Czy sygnał dziecka jest widoczny w dziecku?\n");
                raise(SIGUSR1); // potomek wysyła sygnał do samego siebie
                ifBlocked();
            }
            printf("Czy sygnał rodzica jest widoczny w dziecku? \n"); // czy sygnał w przodku jest widoczny w potomku
            ifBlocked();
        }
        else {wait(NULL);}
    }
    return 0;
}

