#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "common.h"
int table_sem;
int table_shm;
int oven_sem;
int oven_shm;

long long getTime() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}

long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL);
    long long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
    return milliseconds;
}
void bake_pizza(){
    int baked = 0;
    //reserve access to table
    printf("DUPA");
    struct sembuf sops;
    sops.sem_num = 0;
    sops.sem_op = -1;
    sops.sem_flg = 0;

    struct sembuf ret_buf;
    ret_buf.sem_num = 0;
    ret_buf.sem_op = 1;
    ret_buf.sem_flg = 0;
    key_t table_shm_key  = ftok(getenv("home"), ID_T_SHM);
    table_shm = shmget(table_shm_key, 0 ,0 );
    key_t oven_shm_key  = ftok(getenv("home"), ID_O_SHM);
    oven_shm = shmget(oven_shm_key, 0 ,0 );
    while(baked == 0){
        //get access to table
        semop(table_sem, &sops, 1);
        table* t = shmat(table_shm, NULL, 0);
        oven* o = shmat(oven_shm, NULL, 0);
        if (t->pizza > 0){
            int pizza = o->arr[o->to_deliver];
            o->arr[t->to_deliver] = -1;
            o->to_deliver ++;
            o->to_deliver = o->to_deliver % SIZE_T;
            o->pizza --;
            printf("pid: %d timestamp: %lld DODAŁEM PIZZE: %d LICZBA PIZZ W PIECU: %d \n", getpid(),getTime(), pizza, o->pizza);
            baked = 1;
        }
        shmdt(t);
        //return access to table
        semop(table_sem, &ret_buf, 1);
        if (baked == 0){
            sleep(1);
        }

    }
}

void finish_baking(){
    int baked = 0;

    struct sembuf sops;
    sops.sem_num = 0;
    sops.sem_op = -1;
    sops.sem_flg = 0;

    struct sembuf ret_buf;
    ret_buf.sem_num = 0;
    ret_buf.sem_op = 1;
    ret_buf.sem_flg = 0;

    key_t table_shm_key  = ftok(getenv("home"), ID_T_SHM);
    table_shm = shmget(table_shm_key, 0 ,0 );
    key_t oven_shm_key  = ftok(getenv("home"), ID_O_SHM);
    oven_shm = shmget(oven_shm_key, 0 ,0 );
    while(baked == 0){
        semop(table_sem, &sops, 1);
        semop(oven_sem, &sops, 1);
        table* t = shmat(table_shm, NULL, 0);
        oven* o = shmat(oven_shm, NULL, 0);
        if (t->pizza > 0){
            int type = o->arr[o->to_deliver];
            o->arr[o->to_deliver] = -1;
            o->to_deliver ++;
            o->to_deliver = o->to_deliver % SIZE_O;
            o->pizza -- ;
            shmdt(o);
            semop(oven_sem, &ret_buf, 1);
            printf("pid: %d timestamp: %lld WYJMUJE PIZZE: %d LICZBA PIZZ W PIECU: %d LICZBA PIZZ NA STOLE: %d \n", getpid(), getTime(), type, o->pizza, t->pizza);            baked = 1;
            t->arr[t->to_place] = type;
            t->to_place ++;
            t->to_place = t->to_place % SIZE_T;
            t->pizza ++;
        }
        shmdt(t);
        semop(table_sem, &ret_buf, 1);
        if (baked == 0){
            sleep(1);
        }
    }
}

int main (int argc, char* argv[]) {
    key_t table_sem_key = ftok(getenv("home"), ID_T);
    table_sem = semget(table_sem_key, 0,0);
    key_t oven_sem_key = ftok(getenv("home"), ID_O);
    oven_sem = semget(oven_sem_key, 0,0);
    srand(time(NULL));

    while(true){
        int type = rand() % 10;
        printf("pid: %d timestamp: %lld PRZYGOTOWUJE PIZZE: %d \n", getpid(), getTime(), type);
        sleep(2);
        bake_pizza();
        sleep(TIME_B);
    }
    return  0;
}