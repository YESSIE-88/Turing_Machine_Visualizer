#include "turing_machine.h"
#include <stdlib.h>
#include <string.h>

TM_Instance* create_tm(TuringMachine config, char* initial_input) {
    TM_Instance* tm = (TM_Instance*)malloc(sizeof(TM_Instance));
    tm->config = config;
    tm->tape = create_list();
    tm->current_state = config.states[0]; // Start at the first parsed state (usually q0)

    // Load input onto tape
    for (int i = 0; initial_input[i] != '\0'; i++) {
        add_node(tm->tape, initial_input[i]);
    }
    add_node(tm->tape, '_'); // End of input blank

    // Set head to the start of the tape
    tm->read_write_head = tm->tape->head;

    return tm;
}

void print_tm_state(TM_Instance* tm) {
    printf("\nState: q%c\n", tm->current_state);
    printf("Tape: ");
    Node* curr = tm->tape->head;
    while (curr != NULL) {
        if (curr == tm->read_write_head) {
            printf("[%c]", curr->data); // Highlight the head
        } else {
            printf(" %c ", curr->data);
        }
        curr = curr->next;
    }
    printf("\n");
}

int is_halted(TM_Instance* tm) {
    return (tm->current_state == '@' || tm->current_state == '!');
}

int step_tm(TM_Instance* tm) {
    if (is_halted(tm)) return 0;

    char current_char = tm->read_write_head->data;
    Transition* match = NULL;

    // Find the matching transition
    for (int i = 0; i < tm->config.trans_count; i++) {
        if (tm->config.transitions[i].current_state == tm->current_state &&
            tm->config.transitions[i].read_char == current_char) {
            match = &tm->config.transitions[i];
            break;
        }
    }

    // Apply the transition
    // Update the tape
    tm->read_write_head->data = match->write_char;
    
    // Update the state
    tm->current_state = match->next_state;

    // Move the head
    if (match->direction == 'R') {
        if (tm->read_write_head->next == NULL) {
            add_node(tm->tape, '_'); // Extend tape if moving Right at end
        }
        tm->read_write_head = tm->read_write_head->next;
    } 
    else if (match->direction == 'L') {
        if (tm->read_write_head->prev == NULL) {
            // Standard TMs usually halt or stay at leftmost, but we'll prevent a crash here.
            printf("Error: Attempted to move Left past start of tape.\n");
            tm->current_state = '!';
            return 0;
        }
        tm->read_write_head = tm->read_write_head->prev;
    }

    // 'N' (None) results in no pointer change

    return is_halted(tm) ? 0 : 1;
}

void destroy_tm(TM_Instance* tm) {
    if (tm) {
        destroy_list(tm->tape);
        free(tm);
    }
}