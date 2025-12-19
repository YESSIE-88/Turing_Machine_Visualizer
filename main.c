#include <stdio.h>
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
    printf("(Note: qaccept and qreject are implicit)\n");

    // 4. Display Initial Machine State
    printf("-------------------------------------------\n");
    printf("INITIAL MACHINE STATE:");
    print_tm_state(tm);
    printf("===========================================\n");

    // Clean up
    destroy_tm(tm);

    return 0;
}