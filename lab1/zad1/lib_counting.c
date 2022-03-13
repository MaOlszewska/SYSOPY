
// Created by martynka on 08.03.2022.
//
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../zad1/lib_counting.h"

struct ArrayOfPointers* create_array(int size){
    if(size <= 0) return NULL;
    struct ArrayOfPointers* array =calloc(1, sizeof(struct ArrayOfPointers));
    array->size = size;
    array -> blocks = calloc(size, sizeof(struct Block));
    array-> las_idx = -1;
    return array;
}


struct Block* create_counting_blocks(char files[]){
    int counter = 0;
    for(int i = 0; i < strlen(files); i++){
        if(files[i] == ' ')
            counter ++;
    }

    struct Block* block = calloc(1, sizeof(struct Block));
    block -> rows_number = counter ;
    block -> rows = calloc(block->rows_number, sizeof(char *));


    char command[]= {"wc "};
    strcat(command, files);
    FILE* temp = popen(command, "r");
    char line[100];
    for(int i = 0; i < block->rows_number; i++){
        char* row = calloc(256,sizeof(char));
        fgets(line, 256*sizeof(char), temp);
        strcpy(row,line);
        block->rows[i] = row;
    }

    fclose(temp);
    return block;
}

int count_files(struct ArrayOfPointers* array, char*fiels){
    struct Block* block_to_added = create_counting_blocks(fiels);
    array->blocks[++array->las_idx] = block_to_added;
    return array->las_idx;
}

void remove_rows(struct Block* block){
    for(int i = 0; i < block->rows_number; i++){
        if(block->rows[i] != NULL){
            free(block->rows[i]);
            block -> rows[i] = NULL;
        }
    }
    free(block->rows);
    block-> rows = NULL;
}

void remove_block(struct ArrayOfPointers* array, int index){
    if(array->blocks[index] != NULL){
        remove_rows(array->blocks[index]);
        free(array->blocks[index]);
        array->blocks[index] = NULL;
    }
}

void remove_array(struct ArrayOfPointers* array){
    for(int i = 0; i < array->size; i++){
        if(array->blocks[i] != NULL){
            remove_rows(array->blocks[i]);
            free(array->blocks[i]);
        }
    }
    free(array->blocks);
    free(array);
}

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
