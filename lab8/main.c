#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h>
#include <pthread.h>

int W;
int H;
int M;
int ** input_image;
int ** output_image;
int nThreads;

void number_handler(void * args){
    clock_t  real_time[2];
    real_time[0] = clock();
    int numberThread = * (int *) args;
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            if(input_image[i][j] >= (256/nThreads)*numberThread && input_image[i][j] < (256/nThreads)*(numberThread + 1)){
                output_image[i][j] = 255 - input_image[i][j];
            }
        }
    }
    real_time[1] = clock();
    double* execute_time = (double*) calloc(1, sizeof(double));
    *execute_time = (double)(real_time[1]-real_time[0])/CLOCKS_PER_SEC;
    pthread_exit(execute_time);
}

void block_handler(void * args){
    clock_t  real_time[2];
    real_time[0] = clock();
    int numberThread = * (int *) args;
    for(int i = numberThread * W / nThreads; i < (numberThread + 1) * W / nThreads; i++){
        for(int j = 0; j < H; j++){
            if(input_image[i][j] >= (256/nThreads)*numberThread && input_image[i][j] < (256/nThreads)*(numberThread + 1)){
                output_image[i][j] = 255 - input_image[j][i];
            }
        }
    }
    real_time[1] = clock();
    double* execute_time = (double*) calloc(1, sizeof(double));
    *execute_time = (double)(real_time[1]-real_time[0])/CLOCKS_PER_SEC;
    pthread_exit(execute_time);
}

int main (int argc, char* argv[]){

    int type;
    char *input_filename;
    char *output_filename;
    char* id = (char*) calloc(50, sizeof(char));

    if (argc < 5){
        printf("Za mało argumentów :( \n");
        exit(0);
    }

    //pobieranie argumentów programu
    nThreads = atoi(argv[1]);
    if(strcmp((argv[2]), "numbers") == 0){
        type = 1;
    }
    else{
        type = 0;
    }

    input_filename = argv[3];
    output_filename = argv[4];

    // otwieram plik tylko do odczytu bo nie potrzeba mi nic więcej
    FILE *pic = fopen(pic, "r");
    fscanf(pic, "%s", id);
    fscanf(pic, "%d %d", &W, &H);
    fscanf(pic, "%d", &M);

    // tworze tablice z inputem i outputem
    input_image = (int**) calloc(H, sizeof(int*));
    output_image = (int**) calloc(H, sizeof(int*));
    for( int i = 0; i < H; i++){
        input_image[i] = ( int*) calloc(W, sizeof(int));
        output_image[i] = (int*) calloc(W, sizeof(int));
    }

    // zapisanie obrazka do tablicy

    for (int i = 0; i < H; i++){
        for( int j = 0; j < W; j ++){
            fscanf(pic, "%d", input_image[i][j]);
        }
    }

    // zamykam plik
    fclose(input_filename);

    clock_t  real_times[2];
    real_times[0] = clock();

    // tworze wątki
    pthread_t* threadsID = (pthread_t*) malloc(sizeof(pthread_t) * nThreads);
    if( type == 1){
        for(int i = 0; i < nThreads; i++){
            int * numberThread = (int*) malloc(sizeof(int));
            *numberThread = i;
            pthread_create(&threadsID[i], NULL, number_handler, numberThread);
        }
    }
    else{
        for(int i = 0; i < nThreads; i++){
            int * numberThread = (int*) malloc(sizeof(int));
            *numberThread = i;
            pthread_create(&threadsID[i], NULL, block_handler, numberThread);
        }

    }
    // czekam na zakonczenie pracy wszystkich wątków
    double * thread_time;
    for(int i = 0; i < nThreads; i++){
        pthread_join(threadsID[i], (void*)&thread_time);
        printf("Jestem watkiem numer: %d i soją prace wykonałem w czasie: %f\n", i, *thread_time);

    }
    free(thread_time);
    real_times[1] = clock();
    double all_threads_time = (double)(real_times[1] - real_times[0]) / CLOCKS_PER_SEC;
    printf("Program zakończył wszytskie operacje w czsie %f \n", all_threads_time);

    // zapisuje wyniki w outputfile
    FILE* output_pic = fopen(output_filename, "w");
    fprintf(output_pic, "%s\n%d %d\n%d\n", id, W, H, 255);
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            fprintf(output_pic, "%d ", output_image[i][j]);
        }

        fprintf(output_pic, "\n");

    }
    fclose(output_pic);

    // zwalniam całą wcześniej zaalokowaną pamięć
    for(int i = 0; i < H; i++){
        free(input_image[i]);
        free(output_image[i]);
    }
    free(output_image);
    free(input_image);
    return 0;
}
