#ifndef TM_PARSER_H
#define TM_PARSER_H

#include <stdio.h>

#define MAX_ALPHABET 8
#define MAX_STATES 10 
#define MAX_TRANSITIONS 90 // 10 states * 9 possible tape chars

typedef struct {
    char current_state;
    char read_char;
    char next_state;
    char write_char;
    char direction; // 'R', 'L', or 'N'
} Transition;

typedef struct {
    char input_alphabet[MAX_ALPHABET];
    int input_count;

    char tape_alphabet[MAX_ALPHABET + 1];
    int tape_count;

    char states[MAX_STATES];
    int state_count;

    Transition transitions[MAX_TRANSITIONS];
    int trans_count;
} TuringMachine;

void parse_tm_file(const char *filename, TuringMachine *tm);

#endif