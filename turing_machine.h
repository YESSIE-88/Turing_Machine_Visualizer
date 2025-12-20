#ifndef TURING_MACHINE_H
#define TURING_MACHINE_H

#include "dll.h"
#include "tm_parser.h"

typedef struct {
    DoublyLinkedList* tape;
    Node* read_write_head;           // Current position of the read/write head
    char current_state;   // The single char ID (e.g., '0' or 'l')
    TuringMachine config; // The alphabets and state list from the parser
} TM_Instance;

// Function prototypes
TM_Instance* create_tm(TuringMachine config, char* initial_input);
void print_tm_state(TM_Instance* tm);
int is_halted(TM_Instance* tm);
int step_tm(TM_Instance* tm);
void destroy_tm(TM_Instance* tm);

#endif