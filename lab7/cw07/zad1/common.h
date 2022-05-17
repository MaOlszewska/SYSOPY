#define TIME_P 2
#define TIME_B 5
#define TIME_D 4
#define TIME_R 4

#define SIZE_O 5
#define SIZE_T 5

#define ID_O_SHM 'S'
#define ID_T_SHM 'M'
#define ID_O 'O'
#define ID_T 'T'

typedef struct {
    int arr[5];
    int pizza;
    int to_place;
    int to_deliver;
}oven;

typedef struct {
    int arr[5];
    int pizza;
    int to_place;
    int to_deliver;
}table;

union semun {
    int val;
    struct  semid_ds *buf;
    unsigned  short *array;
    struct seminfo* _buf;
} arg;

