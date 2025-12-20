#include <stdio.h>
#include <unistd.h>
#include "tm_parser.h"
#include "turing_machine.h"

int main(int argc, char* argv[]) {
    // Corrected logic: argv[1] is config file, argv[2] is input string
    if (argc < 3) {
        printf("Usage: %s <tm_config_file> <input_string>\n", argv[0]);
        return 1;
    }

    // 1. Parse the text file for alphabets and states
    TuringMachine config = {0};
    parse_tm_file(argv[1], &config);

    // 2. Initialize the Turing Machine "Object" with the input string
    TM_Instance* tm = create_tm(config, argv[2]);

    // 3. Display Detailed Configuration
    printf("===========================================\n");
    printf("   TURNING MACHINE CONFIGURATION LOADED    \n");
    printf("===========================================\n");

    printf("Input Alphabet (%d): { ", config.input_count);
    for(int i = 0; i < config.input_count; i++) {
        printf("%c%s", config.input_alphabet[i], (i == config.input_count - 1) ? "" : ", ");
    }
    printf(" }\n");

    printf("Tape Alphabet  (%d): { ", config.tape_count);
    for(int i = 0; i < config.tape_count; i++) {
        printf("%c%s", config.tape_alphabet[i], (i == config.tape_count - 1) ? "" : ", ");
    }
    printf(" }\n");

    printf("Working States (%d): { ", config.state_count);
    for(int i = 0; i < config.state_count; i++) {
        printf("q%c%s", config.states[i], (i == config.state_count - 1) ? "" : ", ");
    }
    printf(" }\n");
    printf("(Note: the accept and reject states are implicit\n@ is the accept symbol\n! is the reject symbol)\n");

    printf("\nTransitions Loaded (%d):\n", config.trans_count);
    printf("-------------------------------------------\n");
    for(int i = 0; i < config.trans_count; i++) {
        Transition t = config.transitions[i];
        printf("(q%c, %c) -> (q%c, %c, %c)\n", 
                t.current_state, t.read_char, t.next_state, t.write_char, t.direction);
    }
    printf("===========================================\n");

    // 4. Display Initial Machine State
    printf("-------------------------------------------\n");
    printf("INITIAL MACHINE STATE:");
    print_tm_state(tm);
    printf("===========================================\n");

    printf("\nStarting simulation...\n");
    
    int status = 1;
    int step_count = 0;

    // Use a special ANSI escape code to hide the cursor for better visuals
    printf("\e[?25l"); 

    while (status == 1) {
        // \r returns to start of line. \e[K clears the line from cursor to end.
        printf("\r\e[K[Step: %03d] State: q%c | Tape: ", step_count, tm->current_state);
        
        // Manual print loop here to keep it on one line with \r
        Node* curr = tm->tape->head;
        while (curr != NULL) {
            if (curr == tm->read_write_head) printf("[%c]", curr->data);
            else printf(" %c ", curr->data);
            curr = curr->next;
        }

        // Force the terminal to show the buffer immediately
        fflush(stdout);

        // Wait 0.5 seconds (500,000 microseconds)
        usleep(500000);

        status = step_tm(tm);
        step_count++;
        
        if (step_count > 1000) {
            printf("\nSafety limit reached!");
            break;
        }
    }

    // Show the final halting state
    printf("\r\e[K[Step: %03d] State: q%c | Tape: ", step_count, tm->current_state);
    Node* curr = tm->tape->head;
    while (curr != NULL) {
        if (curr == tm->read_write_head) printf("[%c]", curr->data);
        else printf(" %c ", curr->data);
        curr = curr->next;
    }

    if (tm->current_state == '@') printf("\n\n>>> ACCEPTED <<<\n");
    else printf("\n\n>>> REJECTED <<<\n");

    // Show the cursor again
    printf("\e[?25h\n");

    destroy_tm(tm);
    return 0;
}