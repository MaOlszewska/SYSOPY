#define TIME_P 2
#define TIME_B 5
#define TIME_D 4
#define TIME_R 4

#define SIZE_O 5
#define SIZE_T 5

#define OVEN_SHM "/bake_shm"
#define TABLE_SHM "/table_shm"
#define OVEN_SEM "/bake_sem"
#define TABLE_SEM "/table_sem"



union semun {
    int val;
    struct  semid_ds *buf;
    unsigned  short *array;
    struct seminfo* _buf;
} arg;

