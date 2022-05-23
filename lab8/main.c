#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h>
#include <pthread.h>

int width;
int height;
int M;
int ** inputImage;
int ** outputImage;
int numThreads;
pthread_t * threads;

void *number_handler(void * args){
    clock_t  time[2];
    time[0] = clock();
    int numberThread = * (int *) args;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(inputImage[i][j] >= (256 / numThreads) * numberThread && inputImage[i][j] < (256 / numThreads) * (numberThread + 1)){
                outputImage[i][j] = 255 - inputImage[i][j];
            }
        }
    }
    time[1] = clock();
    double* executeTime = (double*) calloc(1, sizeof(double));
    *executeTime = (double)(time[1] - time[0]) / CLOCKS_PER_SEC;
    pthread_exit(executeTime);
}

void *block_handler(void * args){
    clock_t  time[2];
    time[0] = clock();
    int numberThread = * (int *) args;
    for(int i = numberThread * width / numThreads; i < (numberThread + 1) * width / numThreads; i++){
        for(int j = 0; j < height; j++){
            if(inputImage[i][j] >= (256 / numThreads) * numberThread && inputImage[i][j] < (256 / numThreads) * (numberThread + 1)){
                outputImage[i][j] = 255 - inputImage[j][i];
            }
        }
    }
    time[1] = clock();
    double* executeTime = (double*) calloc(1, sizeof(double));
    *executeTime = (double)(time[1] - time[0]) / CLOCKS_PER_SEC;
    pthread_exit(executeTime);
}

int main (int argc, char* argv[]){

    int type;
    char *inputFilename;
    char *outputFilename;
    char* id = (char*) calloc(50, sizeof(char));

    if (argc < 5){
        printf("Za mało argumentów, smuteczek:( \n");
        exit(0);
    }

    //pobieranie argumentów programu
    numThreads = atoi(argv[1]);
    threads = (pthread_t*) calloc(numThreads, sizeof (pthread_t));
    if(strcmp((argv[2]), "numbers") == 0){ type = 1; }
    else{ type = 0; } //block

    inputFilename = argv[3];
    outputFilename = argv[4];

    // otwieram plik tylko do odczytu bo nie potrzeba mi nic więcej
    FILE *pic = fopen(inputFilename, "r");
    if (pic == NULL){
        printf("Mały błędzik :( \n");
        exit(-1);
    }
    // zczytuje i zpaisane dane o zdjęciu
    printf("Zdjęcie wejściowe : %s \n", inputFilename);
    fscanf(pic, "%s", id);
    fscanf(pic, "%d %d", &width, &height);
    fscanf(pic, "%d", &M);

    // tworze tablice z inputem i outputem
    inputImage = (int**) calloc(height, sizeof(int*));
    outputImage = (int**) calloc(height, sizeof(int*));
    for(int i = 0; i < height; i++){
        inputImage[i] = ( int*) calloc(width, sizeof(int));
        outputImage[i] = (int*) calloc(width, sizeof(int));
    }

    // zapisanie obrazka do tablicy

    for (int i = 0; i < height; i++){
        for(int j = 0; j < width; j ++){
            fscanf(pic, "%d", &inputImage[i][j]);
        }
    }

    fclose(pic);

    //rozpoczynam liczenie czasu na wykonanie całego programu
    clock_t  real_times[2];
    real_times[0] = clock();

    // tworze wątki
    if( type == 1){ // dla numbers
        for(int i = 0; i < numThreads; i++){
            int * numberThread = (int*) malloc(sizeof(int));
            *numberThread = i;
            pthread_create(&threads[i], NULL, number_handler, numberThread);
        }
    }
    else{ // dla block
        for(int i = 0; i < numThreads; i++){
            int * numberThread = (int*) malloc(sizeof(int));
            *numberThread = i;
            pthread_create(&threads[i], NULL, block_handler, numberThread);
        }
    }

    // czekam na zakonczenie pracy wszystkich wątków
    double * thread_time;
    for(int i = 0; i < numThreads; i++){
        pthread_join(threads[i], (void*)&thread_time);
        printf("Jestem watkiem numer: %d i swoją prace wykonałem w czasie: %f\n", i, *thread_time);
    }
    free(thread_time);

    // kończe liczenie czasu
    real_times[1] = clock();
    double all_threads_time = (double)(real_times[1] - real_times[0]) / CLOCKS_PER_SEC;
    printf("Program zakończył wszytskie operacje w czsie %f \n", all_threads_time);

    // zapisuje wyniki w outputfile
    FILE* outputPic = fopen(outputFilename, "w");
    fprintf(outputPic, "%d %d\n%d\n", width, height, 255);
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            fprintf(outputPic, "%d ", outputImage[i][j]);
        }

        fprintf(outputPic, "\n");

    }
    fclose(outputPic);

    // zwalniam całą wcześniej zaalokowaną pamięć
    for(int i = 0; i < height; i++){
        free(inputImage[i]);
        free(outputImage[i]);
    }
    free(outputImage);
    free(inputImage);

    free(threads);
    return 0;
}
