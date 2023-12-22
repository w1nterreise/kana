#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define.h"




const char * kana[KANA_SIZE];

typedef struct {
    int sequence[KANA_SIZE];
    int choices[KANA_SIZE][4];
    bool choice_enable[4];
    int pointer;
    int faults;
} model_t;

model_t model;

void choices_unlock_all();
void choises_lock_all();
void shuffle(int array[], size_t n);
void generate_new_sequence();
void generate_answers();
void generate_new_layout();
void print_sequence();
int pick(int option);
