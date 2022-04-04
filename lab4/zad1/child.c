#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

mode_t mode;
char* mode_name;
enum mode_t {IGNORE, HANDLER, MASK, PENDING};

void ifBlocked(){
    sigset_t blocked;
    sigpending(&blocked);  // sprawdza oczekujace sygnały na odblowanie
    if(sigismember(&blocked, SIGUSR1) == 1){ // czy sygnał SIGUSR1 należy do blocked jeśli tak to zwraca 1
        printf("TAK \n");}
    else{printf("NIE \n");}
}


int main(int argc, char* argv[]) {

    mode_name = argv[1];

    if(strcmp(mode_name, "ignore") == 0){mode = IGNORE;}
    else if(strcmp(mode_name, "mask") == 0){mode = MASK;}
    else if(strcmp(mode_name, "pending") == 0){mode = PENDING;}
    else {mode = HANDLER;}

    switch (mode) {
        case IGNORE: {
            printf("Przed wygenerowaniem dziecka\n");
            raise(SIGUSR1);  // potomek generuje sygnał dla samego siebie
            printf("Po wygenerowaniu dziecka \n");
            break;
        }
        case MASK: {

            printf("Czy sygnał dziecka jest widoczny w dziecku?\n");
            raise(SIGUSR1); // potomek wysyła sygnał do samego siebie
            ifBlocked();
            printf("Czy sygnał rodzica jest widoczny w dziecku? \n"); // czy sygnał w przodku jest widoczny w potomku
            ifBlocked();
            break;
        }
        case PENDING:{
            printf("Czy sygnał rodzica jest widoczny w dziecku? \n"); // czy sygnał w przodku jest widoczny w potomku
            ifBlocked();
            break;
        }
    }
    return 0;
}