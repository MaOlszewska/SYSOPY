
// Created by martynka on 08.03.2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../zad1/lib_counting.h"

//
// utworzenie tablicy wskaźników w której będą przechowywane wskaźniki na bloki pamięci zawierające wyniki
//

struct ArrayOfPointers* create_array(int size){
    if(size <= 0) return NULL;
    struct ArrayOfPointers* array =calloc(1, sizeof(struct ArrayOfPointers)); //alokuje pamięćcdla tablicy wskaźników
    array->size = size;                                                 // tablica ma pole size czyli jej rozmiar
    array -> blocks = calloc(size, sizeof(struct Block));   // alokuje pamięć dla wskaźników na bloki z wynikami
    array-> las_idx = -1;                                        // pole które zawiera ostatni indeks

    printf("Array was made \n");
    return array;
}


//
// przeprowadzenie zliczenia lini, słów i znaków dla zadanych plików i zapisanie wyniku zliczania w pliku tymczasowym
// arezerwowanie bloku pamięci o rozmiarze odpowiadającym rozmiarowi pliku tymczasowego i zapisanie w tej
// pamięci jego zawartości
//

struct Block* create_counting_blocks(char files[]){
    int counter = 0;
    for(int i = 0; i < strlen(files); i++){
        if(files[i] == ' ')
            counter ++;
    }

    struct Block* block = calloc(1, sizeof(struct Block)); // Alokuje pamięć dla bloków pamięci
    block -> rows_number = counter + 1; // obliczam ile lini bedzie posiadać wynik
    block -> rows = calloc(block->rows_number, sizeof(char *)); // alokuje pamięć,aby zapisać linie wyniku


    char command[]= {"wc "};
    strcat(command, files);   // tworze komendę do przekazania do wywołania
//    FILE* temp = tmpfile();
//    strcat(command, " > temp_file.txt");
//    system(command);                          //zapisanie danych do pliku
    FILE* temp = popen(command, "r");    // Wywołuje polecenie zliczania lini, słów i znaków i zapisuje do pliku temp
    char line[100];
    for(int i = 0; i < block->rows_number; i++){
        char* row = calloc(256,sizeof(char));
        fgets(line, 256*sizeof(char), temp); // zapisuje każdą linie do bloku zarezerwowanej pamięci
        strcpy(row,line);
        block->rows[i] = row;
    }

    fclose(temp);

    printf("blocks are made \n");
    return block;
}

//
//ustawienie w tablicy wskaźników wskazania na ten blok, funkcja powinna zwrócić indeks stworzonego bloku w tablicy,
//

int count_files(struct ArrayOfPointers* array, char*fiels){
    struct Block* block_to_added = create_counting_blocks(fiels);
    array->blocks[++array->las_idx] = block_to_added;
    printf("Last index is %d \n", array->las_idx );
    return array->las_idx;
}


//
// usunięcie z bloku pamieci wszytskich wierszy
//

void remove_rows(struct Block* block){
    for(int i = 0; i < block->rows_number; i++){
        if(block->rows[i] != NULL){
            printf("Removed rows %d\n", i);
            free(block->rows[i]);
            block -> rows[i] = NULL;
        }
    }
    free(block->rows);
    block-> rows = NULL;
}

//
// usunięcie z pamięci bloku o zadanym indeksie
//

void remove_block(struct ArrayOfPointers* array, int index){
    if(array->blocks[index] != NULL){
        remove_rows(array->blocks[index]);
        free(array->blocks[index]);
        array->blocks[index] = NULL;
        printf("Removed block %d \n", index);
    }
}

void remove_array(struct ArrayOfPointers* array){
    for(int i = 0; i < array->size; i++){
        if(array->blocks[i] != NULL){
            remove_rows(array->blocks[i]);
            free(array->blocks[i]);
            printf("Removed block %d \n", i);
        }
    }
    free(array->blocks);
    free(array);
    printf("Removed array \n");
}



//
// funckje pomocnicze wypisujące zawartość
//

void show_block(struct Block* block){
    if(block == NULL){
        printf("Nothing to remove \n");
    }
    else{
        for(int i = 0; i < block->rows_number; i++){
            printf("Row number %d -> %s \n", i,block->rows[i]);
        }
    }
    printf("\n");
}

void show_array(struct ArrayOfPointers* array){
    struct Block *block;
    for(int i = 0; i < array->size; i++){
        block = array->blocks[i];
        if( block != NULL){
            printf("Block number %d: \n ",i);
            show_block(block);
        }
    }
}


// todo
// poprawić usuwanie całego array   ------ chyba ok
// valgrind   ---------- mam
// przesuwanie indeksów