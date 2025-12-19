#ifndef TM_PARSER_H
#define TM_PARSER_H

#include <stdio.h>

#define MAX_ALPHABET 9
#define MAX_STATES 8

typedef struct {
    char input_alphabet[MAX_ALPHABET];
    int input_count;

    char tape_alphabet[MAX_ALPHABET + 1]; // Including blank symbol
    int tape_count;

    char states[MAX_STATES]; // Stores only the char after 'q'
    int state_count;
} TuringMachine;

void parse_tm_file(const char *filename, TuringMachine *tm);

#endif