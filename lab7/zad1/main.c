#include <stdlib.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

#include "common.h"

int semafor_oven, semafor_table;
int shm_oven, shm_table;
pid_t deliveres_pid[10];
pid_t pizzaiols_pid[10];

void create_shm(int p, int tab){
    key_t oven_shm_key = ftok(getenv("home"), ID_O_SHM);
    shm_oven =  shmget(oven_shm_key, SIZE_O, IPC_CREAT | 0666);

    key_t table_shm_key = ftok(getenv("home"), ID_T_SHM);
    shm_table =  shmget(table_shm_key, SIZE_T, IPC_CREAT | 0666);


    oven* o = shmat(shm_oven, NULL, 0);
    table* t = shmat(shm_table, NULL, 0);

    for(int i = 0; i < p; i++){o->arr[i] = -1;}
    for(int i = 0; i < tab; i++){t->arr[i] = -1;}
    t->pizza = 0;
    o->pizza = 0;
    t->to_deliver = 0;
    o->to_deliver = 0;
    o->to_place = 0;
    t->to_place = 0;

    shmdt(o);
    shmdt(t);
}

void create_sem() {
    key_t oven_sem_key = ftok(getenv("home"), ID_O);
    semafor_oven = semget(oven_sem_key, 2, IPC_CREAT | 0666);

    key_t table_sem_key = ftok(getenv("home"), ID_T);
    semafor_table = semget(table_sem_key, 2, IPC_CREAT | 0666);

    union semun arg;
    arg.val = 1;
    semctl(semafor_oven, 0 , SETVAL, arg);
    semctl(semafor_table, 0, SETVAL, arg);
}


void handler(int sig){
    if(sig == SIGINT){
        printf("PIZZERIA IS CLOSE\n");
        for (int i = 0; i < 10; i ++){
            kill(pizzaiols_pid[i], SIGINT);
        }

        for (int i = 0; i < 10; i ++){
            kill(deliveres_pid[i], SIGINT);
        }

        printf("PIZZERIA IS CLOSE\n");
        shmctl(shm_table, IPC_RMID, NULL);
        shmctl(shm_oven, IPC_RMID, NULL);
        shmctl(semafor_oven, IPC_RMID, NULL);
        shmctl(semafor_table, IPC_RMID, NULL);
        exit(0);
    }
}
int main (int argc, char* argv[]){
    signal(SIGINT, handler);
    int pizzaiolos,deliverers;
    if(argc == 3){
        deliverers = atoi(argv[2]);
        pizzaiolos = atoi(argv[1]);
    }
    else{
        printf("Not enough arg :( \n");
        exit(-1);
    }
    // Inicjalizuje pamięć dzieloną

    create_shm(pizzaiolos, deliverers);
    create_sem();


    printf("PIZZERIA IS OPEN\n");
    for (int i = 0; i < pizzaiolos; i ++){
        pizzaiols_pid[i] = fork();
        if(pizzaiols_pid[i] == 0){
            execl("./pizza", "./pizza", NULL);
        }
    }

    for (int i = 0; i < deliverers; i ++){
        deliveres_pid[i] = fork();
        if(deliveres_pid[i]== 0){
            execl("./deliver", "./deliver", NULL);
        }
    }

    for(int i =0; i < pizzaiolos + deliverers; i++){
        wait(NULL);
    }

    for (int i = 0; i < pizzaiolos; i ++){
        kill(pizzaiols_pid[i], SIGINT);
    }

    for (int i = 0; i < deliverers; i ++){
        kill(deliveres_pid[i], SIGINT);
    }

    printf("PIZZERIA IS CLOSE\n");
    shmctl(shm_table, IPC_RMID, NULL);
    shmctl(shm_oven, IPC_RMID, NULL);
    shmctl(semafor_oven, IPC_RMID, NULL);
    shmctl(semafor_table, IPC_RMID, NULL);
    return 0;
}