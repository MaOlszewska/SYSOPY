#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>


#include "common.h"


int shm_oven, shm_table;

sem_t * oven_sem;
sem_t * table_sem;

pid_t deliveres_pid[10];
pid_t pizzaiols_pid[10];

void create_shm(int p, int tab){
    shm_oven = shm_open(OVEN_SHM, O_CREAT | O_RDWR, 0760);
    ftruncate(shm_oven, SIZE_O * sizeof (int));

    shm_table = shm_open(TABLE_SHM, O_CREAT | O_RDWR, 0760);
    ftruncate(shm_table, SIZE_T * sizeof (int));

    int * o = (int*) mmap(NULL, SIZE_O, PROT_READ | PROT_WRITE, MAP_SHARED, shm_oven, 0);
    int* t = (int*) mmap(NULL, SIZE_T, PROT_READ | PROT_WRITE, MAP_SHARED, shm_table, 0);


    for(int i = 0; i < p; i++){o[i] = -1;}
    for(int i = 0; i < tab; i++){t[i] = -1;}

    munmap(t, SIZE_T * sizeof (int));
    munmap(o,SIZE_O * sizeof (int));
    close(shm_oven);
    close(shm_table);
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
        shm_unlink(OVEN_SHM);
        shm_unlink(TABLE_SHM);
        sem_close(oven_sem);
        sem_close(table_sem);
        sem_unlink(OVEN_SEM);
        sem_unlink(TABLE_SEM);
        exit(0);
    }
}
int main (int argc, char* argv[]){
    signal(SIGINT, handler);
    int pizzaiolos, deliverers;
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

    oven_sem = sem_open(OVEN_SEM, O_CREAT, 0666,1);
    table_sem = sem_open(TABLE_SEM, O_CREAT, 0666,1);

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



    return 0;
}