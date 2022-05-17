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
#include <stdbool.h>


#include "common.h"

int oven_shm;
int table_shm;
sem_t *oven_sem;
sem_t * table_sem;


long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL);
    long long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
    return milliseconds;
}

int n_pizzas_table(int* table){
    int counter = 0;
    for (int i = 0; i <SIZE_T; i++){
        if (table[i] != -1)
            counter++;
    }
    return counter;
}

int free_index_table(int* table){
    for (int i = 0; i < SIZE_T; i++){
        if (table[i] == -1)
            return i;
    }
    return -1;
}

int first_pizza_id(int* table){
    for (int i = 0; i < SIZE_O; i++){
        if (table[i] != -1)
            return i;
    }
    return -1;
}

int bake_pizza(){
    int took = 0;
    int idx = -1;
    int pizza = -1;
    while(took == 0){
        // dekrementuje warotśc semafora
        sem_wait(oven_sem);
        oven_shm = shm_open(TABLE_SHM, O_CREAT | O_RDWR, 0760);
        // określa rozmair
        ftruncate(oven_shm, SIZE_T * sizeof (int));
        // dołącza segment do przestrzeni adresowej
        int * o  = mmap(NULL, SIZE_O, PROT_READ | PROT_WRITE, MAP_SHARED, oven_shm, 0);
        if (n_pizzas_table(o) > 0){
            int idx = first_pizza_id(o);
            pizza = o[idx];
            int n_pizzas = n_pizzas_table(o);
            printf("pid: %d timestamp: %lld POBIERAM PIZZE NUMER: %d, LICZBA PIZZ NA STOLE: %d \n", getpid(),current_timestamp(), pizza, n_pizzas);
            took = 1;
        }
        // odłącza segemnt od przestzreni adresowej
        munmap(o, SIZE_O * sizeof (int));

        // inkrementuje wartość semafora
        sem_post(oven_sem);
    }
    return idx;
}

void finish_baking(int p){
    int baked = 0;

    sem_wait(oven_sem);
    oven_shm = shm_open(OVEN_SHM, O_CREAT | O_RDWR, 0760);
    // określa rozmiar
    ftruncate(oven_shm, SIZE_O * sizeof (int));
    // dołącza segemnt do przetsrzeni adresowje
    int * o  = mmap(NULL, SIZE_O, PROT_READ | PROT_WRITE, MAP_SHARED, oven_shm, 0);
    int pizza = o[p];
    o[p] =-1;
    int pizzas = n_pizzas_table(o);
    // odłącza segemnt od przetsrzeni adresowej
    munmap(o, SIZE_O* sizeof (int));
    close(oven_shm);

    while(baked == 0){
        // dekrementuje warotsć semafora
        sem_wait(table_sem);
        table_shm = shm_open(TABLE_SHM, O_CREAT | O_RDWR, 0760);
        ftruncate(table_shm, SIZE_T * sizeof (int));
        int * t  = mmap(NULL, SIZE_O, PROT_READ | PROT_WRITE, MAP_SHARED, table_shm, 0);
        if (n_pizzas_table(t) < SIZE_T){
            int idx = free_index_table(t);
            t[idx] = pizza;
            int n_pizzas = n_pizzas_table(t);
            printf("pid: %d timestamp: %lld POBIERAM PIZZE NUMER: %d, LICZBA PIZZ W PIEKARNIKU %d:, LICZBA PIZZ NA STOLE %d \n", getpid(),current_timestamp(), pizza,pizzas, n_pizzas );
            baked = 1;
        }
        // odłącza segemnt od pamięci adresowej
        munmap(t, SIZE_T* sizeof (int));
        close(table_shm);
        // inkrementuje waartość semafora
        sem_post(table_sem);
    }
}

int main (int argc, char* argv[]) {

    srand(time(NULL));
    table_sem = sem_open(OVEN_SEM, 0);
    table_sem = sem_open(TABLE_SEM, 0);
    while(true){
        int type = rand() % 10;
        printf("%d\n", type);
        printf("pid: %d timestamp: %lld PRZYGOTOWUJE PIZZE: %d \n", getpid(), current_timestamp(), type);
        sleep(TIME_P);
        int pizza = bake_pizza();
        sleep(TIME_B);
        finish_baking(pizza);

    }
    return  0;
}