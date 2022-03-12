//
// Created by martynka on 08.03.2022.
//

#ifndef UNTITLED_LIB_COUNTING_H
#define UNTITLED_LIB_COUNTING_H
#include <stdio.h>

struct Block{
    int rows_number;
    char **rows;
};

struct ArrayOfPointers{
    int size;
    int las_idx;
    struct Block** blocks;
};

struct ArrayOfPointers* create_array(int size);

struct Block* create_counting_blocks( char *files);

void remove_block(struct ArrayOfPointers* array, int index);

int count_files(struct ArrayOfPointers* array, char* fiels);

void remove_array(struct ArrayOfPointers* array);

void remove_rows(struct Block* block);

void show_block(struct Block* block);

void show_array(struct ArrayOfPointers* array);

#endif //UNTITLED_LIB_COUNTING_H