#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#define RENIFERY 9
#define ELFY 10
#define WYCIECZKI 3

int wycieczki = 0;
int elfy = 0;
int renifery = 0;
int elfyWracajace = 0;

pthread_mutex_t mutexs;
pthread_cond_t elf_cond;
pthread_cond_t renifer_cond;
pthread_cond_t mikolaj_cond;


void * elf_handler(void* arg){
    while(true){
        sleep(rand() % 3 + 3);
        // blokuje obiekt przez wątek usypiam obecny wątek, aż mutex zosatnie odblokowany
        pthread_mutex_lock(&mutexs);
        if(elfy >= 3 || elfyWracajace > 0){
            printf("ELF: CZEKA NA POWRÓT ELFÓW, %ld \n", pthread_self());
            while(elfy >= 3 || elfyWracajace > 0){
                // ustawiam wątek w czas oczekiwane, kiedy mutez jest odblokowany
                pthread_cond_wait(&elf_cond, &mutexs);
            }
        }

        elfy += 1;
        printf("ELF: CZEKA %d ELFÓW NA MIKOŁAJA, %ld \n", elfy, pthread_self());

        if(elfy == 3){
            printf("ELF: WYBUDZAM MIKOŁAJA, %ld \n", pthread_self());
            // powiadamiam tylko jeden wątek
            pthread_cond_signal(&mikolaj_cond);
        }
        while (elfyWracajace == 0){
            // ustawiam wątek w czas oczekiwane, kiedy mutez jest odblokowany
            pthread_cond_wait(&elf_cond, &mutexs);
        }
        elfyWracajace -= -1;
        printf("ELF: MIKOŁAJ ROZWIĄZUJE MÓJ PROBLEM, %ld \n", pthread_self());
        printf("ELF: WRACAM DO PRACY, %ld \n", pthread_self());
        // odblokowuję mutex
        pthread_mutex_unlock(&mutexs);
    }
    return NULL;
}

void * renifer_handler(void* arg){

    while(true){
        // renifer jest na wakacjach
        sleep(rand() % 6 + 5);
        // blokuje obiekt przez wątek usypiam obecny wątek, aż mutex zosatnie odblokowany
        pthread_mutex_lock(&mutexs);
        renifery += 1;
        printf("RENIFER: CZEKA %d RENIFERÓW NA MIKOŁAJA, %ld \n ", renifery, pthread_self());

        if(renifery == RENIFERY){
            printf("RENIFER: WYBUDZAM MIKOŁAJA, %ld \n", pthread_self());
            // powiadamiam tylko jeden wątek
            pthread_cond_signal(&mikolaj_cond);
        }

        while(renifery > 0){
            // ustawiam wątek w czas oczekiwane, kiedy mutez jest odblokowany
            pthread_cond_wait(&renifer_cond, &mutexs);
        }
        printf("RENIFER: LECE NA WAKACJE, %ld \n", pthread_self());

        // odblokowuję mutex
        pthread_mutex_unlock(&mutexs);
    }
    return NULL;
}

void * mikolaj_handler(void* arg){
    while(wycieczki < WYCIECZKI){
        // blokuje obiekt przez wątek usypiam obecny wątek, aż mutex zosatnie odblokowany
        pthread_mutex_lock(&mutexs);

        while(renifery < RENIFERY && elfy != 3){
            // ustawiam wątek w czas oczekiwane, kiedy mutez jest odblokowany
            pthread_cond_wait(&mikolaj_cond, &mutexs);
        }
        if(renifery == RENIFERY){
            printf("MIKOŁAJ: DOSTARCZAM ZABWKI, %ld \n", pthread_self());
            sleep(rand() % 1 + 3);
            renifery = 0;
            // powiadamian wszytskie oczekujące wątki
            pthread_cond_broadcast(&renifer_cond);
            wycieczki += 1;
        }
        else if (elfy == 3){
            printf("MIKOŁAJ: ROZWIĄZUJE PROBLEMY ELFÓW, %ld \n", pthread_self());
            sleep(rand() % 1 + 2);
            elfy = 0;
            elfyWracajace += 3;
            // powiadamian wszytskie oczekujące wątki
            pthread_cond_broadcast(&elf_cond);
        }

        printf("MIKOŁAJ: ZASYPIAM< %ld \n", pthread_self());
        // odblokowuję mutex
        pthread_mutex_unlock(&mutexs);
    }
    exit(0);
    return NULL;
}


int main (int argc, char* argv[]) {
    srand(time(NULL));
    int numThreads = RENIFERY + ELFY + 1;
    // Tworzę tablice do przechowywania inofmracji o wątkach
    pthread_t threads[numThreads];
    // inicjalizacja mutexu
    pthread_mutex_init(&mutexs, NULL);
    // inicjalizajc azminenych warunkowych
    pthread_cond_init(&elf_cond, NULL);
    pthread_cond_init(&renifer_cond, NULL);
    pthread_cond_init(&mikolaj_cond, NULL);

    // tworzenie wątków
    int i = 0;
    while( i < numThreads){
        if(i < ELFY){pthread_create(&threads[i], NULL, &elf_handler, NULL);}
        else if ( i >= ELFY && i < RENIFERY + ELFY){pthread_create(&threads[i], NULL, &renifer_handler, NULL);}
        else{pthread_create(&threads[i], NULL, &mikolaj_handler, NULL);}
        i++;
    }

    // zbieram wartości zwracane przez wątki
    for(int i = 0; i < numThreads; i++){ pthread_join(threads[i], NULL); }

    // usuwam zmienne warunkowe i mutex
    pthread_mutex_destroy(&mutexs);
    pthread_cond_destroy(&elf_cond);
    pthread_cond_destroy(&renifer_cond);
    pthread_cond_destroy(&mikolaj_cond);

    return 0;
}