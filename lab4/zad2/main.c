#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

void SIGINT_handler(int sig, siginfo_t *info, void *ctx) {
    printf("---SYGNAŁ SIGINT--- \n");
    printf("Nr: %d\n", info->si_signo);
    printf("Status: %d\n", info->si_status);
    printf("PID: %d\n", info->si_pid);
}

void SIGTSTP_handler(int sig, siginfo_t *info, void *ctx) {
    printf("---SYGNAŁ SIGTSTP--- \n");
    printf("Nr: %d\n", info->si_signo);
    printf("Status: %d\n", info->si_status);
    printf("PID: %d\n", info->si_pid);
}

void SIGCHLD_handler(int sig, siginfo_t *info, void *ctx) {
    printf("---SYGNAŁ SIGCHLD--- \n");
    printf("Nr: %d\n", info->si_signo);
    printf("Status: %d\n", info->si_status);
    printf("PID: %d\n", info->si_pid);
}

void NOCLDSTOP_handler(int signal){
    printf("---SYGNAŁ NOCLDSTOP--- \n");
    printf("Nr: %d\n", signal);
    printf("PID: %d\n", getpid());
}

void RESETHAND_handler(int signal){
    printf("---FLAGA RESETHAND--- \n");
    printf("Nr: %d\n", signal);
    printf("PID: %d\n", getpid());
}


void testSIGINT(){
    struct sigaction action_SIG;
    sigemptyset(&action_SIG.sa_mask); // inicjalizuje zestaw sygnałów przekazywany przez set, mają być dodane do maski
    action_SIG.sa_sigaction = SIGINT_handler;
    sigaction(SIGINT, &action_SIG, NULL);  // zmiana akcji po otrzymaniu sygnału
    raise(SIGINT);  // SIGINT- sygnał przerwania
}

void testSIGTSTP(){
    struct sigaction action_SIG;
    sigemptyset(&action_SIG.sa_mask); // inicjalizuje zestaw sygnałów przekazywany przez set, mają być dodane do maski
    action_SIG.sa_sigaction = SIGTSTP_handler;
    sigaction(SIGTSTP, &action_SIG, NULL);  // zmiana akcji po otrzymaniu sygnału
    raise(SIGTSTP);  // SIGTSTP- sygnał  po naciscnieu ctrl+z
}

void testSIGCHLD(){
    struct sigaction action_SIG;
    sigemptyset(&action_SIG.sa_mask); // inicjalizuje zestaw sygnałów przekazywany przez set, mają być dodane do maski
    action_SIG.sa_sigaction = SIGCHLD_handler;
    sigaction(SIGCHLD, &action_SIG, NULL);  // zmiana akcji po otrzymaniu sygnału
    if(fork() == 0){
        exit(0);
    }
    wait(NULL);
}


void testNOCLDSTOP(){
    printf("---SYGNAŁ SIGCHLD FLAGA SA_NOCLDSTOP---\n");
    printf("Wiadomość nie powinna zosatć wysłana\n");
    struct sigaction action_SIG;
    sigemptyset(&action_SIG.sa_mask); // inicjalizuje zestaw sygnałów przekazywany przez set, mają być dodane do maski
    action_SIG.sa_handler = NOCLDSTOP_handler;
    action_SIG.sa_flags = SA_NOCLDSTOP;
    sigaction(SIGCHLD, &action_SIG, NULL);  // zmiana akcji po otrzymaniu sygnału
    if(fork() == 0){
        kill(getpid(),SIGSTOP);
    }
    printf("Nie została wysłana \n");
}

void testONESHOT(){
    struct sigaction action_SIG;
    sigemptyset(&action_SIG.sa_mask); // inicjalizuje zestaw sygnałów przekazywany przez set, mają być dodane do maski
    action_SIG.sa_handler = RESETHAND_handler;
    action_SIG.sa_flags = SA_RESETHAND;
    sigaction(SIGCHLD, &action_SIG, NULL);  // zmiana akcji po otrzymaniu sygnału
    if(fork() == 0){
        exit(0);
    }
    wait(NULL);

    printf("Teraz nie powinno się nic pojwić\n");

    if(fork() == 0){
        exit(0);
    }
    wait(NULL);
    printf("Nic sie nie pojawiło \n");
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("Przykro mi, nie mam argumnetu :( ");
        return -1;
    }
    if(strcmp(argv[1], "SIGINFO") == 0) { //informacje o generowaniu sygnału
       testSIGINT();
       testSIGCHLD();
       testSIGTSTP();
    }
    else if(strcmp(argv[1], "NOCLDSTOP") == 0){
        testNOCLDSTOP(); // czy rodzic otrzyma informacje o zatrzymaniu
    }
    else if(strcmp(argv[1], "RESETHAND") == 0){
        testONESHOT();
    }
    else{
        printf("Zły argument");
    }
    return 0;
}
