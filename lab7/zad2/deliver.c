#include <stdio.h>
#include <sys/sem.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

#include "common.h"

int table_shm;
int shm_table;

sem_t* table_sem;
long long getTime() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}

int n_pizzas_table(int* table){
    int counter = 0;
    for (int i = 0; i <SIZE_T; i++){
        if (table[i] != -1)
            counter++;
    }
    return counter;
}

int first_pizza_id(int* table){
    for (int i = 0; i < SIZE_O; i++){
        if (table[i] != -1)
            return i;
    }
    return -1;
}

int take(){
    int took = 0;
    int pizza = 0;
    while(took == 0){
        sem_wait(table_sem);
        shm_table = shm_open(TABLE_SHM, O_CREAT | O_RDWR, 0760);
        ftruncate(shm_table, SIZE_T * sizeof (int));
        int * t  = mmap(NULL, SIZE_O, PROT_READ | PROT_WRITE, MAP_SHARED, shm_table, 0);
        if (n_pizzas_table(t) < SIZE_T){
            int idx = first_pizza_id(t);
            pizza = t[idx];
            int n_pizzas = n_pizzas_table(t);
            printf("pid: %d timestamp: %lld POBIERAM PIZZE NUMER: %d, LICZBA PIZZ NA STOLE: %d \n", getpid(),getTime(), pizza, n_pizzas);
            took = 1;
        }
        munmap(t, SIZE_T * sizeof (int));
        close(table_shm);
        sem_post(table_sem);
    }
    return pizza;
}

int main (int argc, char* argv[]) {
    srand(time(NULL));
    table_sem = sem_open(TABLE_SEM, 0);
    while(1){
        int type = take();
        sleep(TIME_D);
        printf("pid: %d timestamp: %lld DOSTARCZAM PIZZE: %d\n", getpid(), getTime(), type);
        sleep(TIME_R);
    }

    return 0;
}