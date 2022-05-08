#include <stdlib.h>
#include <pwd.h>
#include <sys/time.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "common.h"
int table_sem;
int table_shm;
char* getTime(){
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    return (char *) timeinfo;
}

int take(){
    int took = 0;
    int pizza = 0;
    //reserve access to table
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
    while(took == 0){
        //get access to table
        semop(table_sem, &sops, 1);
        table* t = shmat(table_shm, NULL, 0);
        if (t->pizza > 0){
            pizza = t->arr[t->to_deliver];
            t->arr[t->to_deliver] = -1;
            t->to_deliver ++;
            t->to_deliver = t->to_deliver % SIZE_T;
            t->pizza --;
            printf("pid: %d timestamp: %s POBIERAM PIZZE NUMER: %d, LICZBA PIZZ NA STOLE: %d \n", getpid(),getTime(), pizza, t->pizza);
            took = 1;
        }
        shmdt(t);
        //return access to table
        semop(table_sem, &ret_buf, 1);
        if (took == 0){
            sleep(1);
        }
    }
    return pizza;
}

int main (int argc, char* argv[]) {
    key_t table_sem_key = ftok(getenv("home"), ID_T);
    table_sem = semget(table_sem_key, 0,0);
    srand(time(NULL));
    while(true){
        int type = take();
        sleep(TIME_D);
        printf("pid: %d timestamp: %s DOSTARCZAM PIZZE: %d)", getpid(), getTime(), type);
        sleep(TIME_R);
    }

    return 0;
}