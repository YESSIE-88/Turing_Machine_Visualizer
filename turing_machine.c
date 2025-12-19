#include "turing_machine.h"
#include <stdlib.h>
#include <string.h>

TM_Instance* create_tm(TuringMachine config, char* initial_input) {
    TM_Instance* tm = (TM_Instance*)malloc(sizeof(TM_Instance));
    tm->config = config;
    tm->tape = create_list();
    tm->current_state = config.states[0]; // Start at the first parsed state (usually q0)
    tm->is_halted = 0;
    tm->accepted = 0;

    // Load input onto tape
    for (int i = 0; initial_input[i] != '\0'; i++) {
        add_node(tm->tape, initial_input[i]);
    }
    add_node(tm->tape, '_'); // End of input blank

    // Set head to the start of the tape
    tm->head = tm->tape->head;

    return tm;
}

void print_tm_state(TM_Instance* tm) {
    printf("\nState: q%c\n", tm->current_state);
    printf("Tape: ");
    Node* curr = tm->tape->head;
    while (curr != NULL) {
        if (curr == tm->head) {
            printf("[%c]", curr->data); // Highlight the head
        } else {
            printf(" %c ", curr->data);
        }
        curr = curr->next;
    }
    printf("\n");
}

void destroy_tm(TM_Instance* tm) {
    if (tm) {
        destroy_list(tm->tape);
        free(tm);
    }
}