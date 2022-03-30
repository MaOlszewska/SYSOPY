#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

void ifBlocked(){
    sigset_t blocked;
    sigpending(&blocked);  // sprawdza oczekujace sygnały
    if(sigismember(&blocked, SIGUSR1) == 1){ // czy sygnał SIGUSR1 należy do blocked jeśli tak to zwraca 1
        printf("TAK \n");}
    else{printf("NIE \n");}
}


int main(int argc, char* argv[]) {

    if (strcmp(argv[1], "ignore") == 0) {
        printf("Przed wygenerowaniem dziecka\n");
        raise(SIGUSR1);  // potomek generuje sygnał dla samego siebie
        printf("Po wygenerowaniu dziecka \n");
    }

    else if (strcmp(argv[1], "mask") == 0 || strcmp(argv[1], "pending") == 0  ) {
        if(strcmp(argv[1], "mask") == 0) {
            printf("Czy sygnał dziecka jest widoczny w dziecku?\n");
            raise(SIGUSR1); // potomek wysyła sygnał do samego siebie
            ifBlocked();
        }
        printf("Czy sygnał rodzica jest widoczny w dziecku? \n"); // czy sygnał w przodku jest widoczny w potomku
        ifBlocked();
    }

    return 0;
}