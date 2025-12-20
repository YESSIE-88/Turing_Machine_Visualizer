#include "tm_parser.h"
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

// Helper to check if a char exists in a specific alphabet array
bool is_in_alphabet(char c, char *alphabet, int count) {
    for (int i = 0; i < count; i++) {
        if (alphabet[i] == c) return true;
    }
    return false;
}

// Helper to check if a state char is valid (working state, @, or !)
bool is_valid_state(char s, char *states, int count) {
    if (s == '@' || s == '!') return true;
    for (int i = 0; i < count; i++) {
        if (states[i] == s) return true;
    }
    return false;
}

void parse_tm_file(const char *filename, TuringMachine *tm) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening TM file");
        return;
    }

    char line[256];
    bool input_alphabet_read = false;
    bool tape_alphabet_read = false;
    bool states_read = false;

    while (fgets(line, sizeof(line), file)) {
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r' || isspace(line[0])) continue;

        // 1. Parse Input Alphabet
        if (strstr(line, "input_alphabet")) {
            char *p = strchr(line, '{');
            while (p && *p != '}' && tm->input_count < 8) {
                if (isalnum(*p)) tm->input_alphabet[tm->input_count++] = *p;
                p++;
            }
            input_alphabet_read = true;
        } 
        // 2. Parse Tape Alphabet
        else if (strstr(line, "tape_alphabet")) {
            char *p = strchr(line, '{');
            while (p && *p != '}' && tm->tape_count < 9) {
                if (isalnum(*p) || *p == '_') tm->tape_alphabet[tm->tape_count++] = *p;
                p++;
            }
            tape_alphabet_read = true;
        } 
        // 3. Parse States
        else if (strstr(line, "states")) {
            // Safety: Ensure alphabets were read first
            if (!input_alphabet_read || !tape_alphabet_read) {
                fprintf(stderr, "Error: 'states' defined before alphabets in %s\n", filename);
                exit(1);
            }
            char *p = strchr(line, '{');
            while (p && *p != '}' && tm->state_count < 8) {
                if (*p == 'q') {
                    char next = *(p + 1);
                    if (next != '@' && next != '!' && next != ' ' && next != '}' && next != ',') {
                        tm->states[tm->state_count++] = next;
                    }
                }
                p++;
            }
            states_read = true;
        }
        // 4. Parse Transitions
        else if (strchr(line, '-') && strchr(line, '>')) {
            // Safety: Ensure all metadata is present before processing rules
            if (!states_read) {
                fprintf(stderr, "Error: Transition found before states/alphabets are defined!\n");
                exit(1);
            }

            char curr_q, read_c, next_q, write_c, dir;
            // The leading space in " %c" skips any leftover whitespace/newlines
            int matches = sscanf(line, " q%c %c -> q%c %c %c", &curr_q, &read_c, &next_q, &write_c, &dir);
            
            Transition t = {0};
            if (matches == 5) {
                t = (Transition){curr_q, read_c, next_q, write_c, dir};
            } else if (matches == 3) {
                // If it's a halt transition, write back what was read and move 'None'
                t = (Transition){curr_q, read_c, next_q, read_c, 'N'};
            } else { 
                continue; 
            }

            // --- STRICT VALIDATION ---
            if (!is_valid_state(t.current_state, tm->states, tm->state_count)) {
                fprintf(stderr, "Error: Transition uses unknown start state 'q%c'\n", t.current_state);
                exit(1);
            }
            if (!is_valid_state(t.next_state, tm->states, tm->state_count)) {
                fprintf(stderr, "Error: Transition goes to unknown state 'q%c'\n", t.next_state);
                exit(1);
            }
            if (!is_in_alphabet(t.read_char, tm->tape_alphabet, tm->tape_count)) {
                fprintf(stderr, "Error: Read character '%c' (ASCII %d) not in tape alphabet\n", t.read_char, t.read_char);
                exit(1);
            }
            if (!is_in_alphabet(t.write_char, tm->tape_alphabet, tm->tape_count)) {
                fprintf(stderr, "Error: Write character '%c' not in tape alphabet\n", t.write_char);
                exit(1);
            }
            if (t.direction != 'R' && t.direction != 'L' && t.direction != 'N') {
                fprintf(stderr, "Error: Invalid direction '%c' in rule q%c %c\n", t.direction, t.current_state, t.read_char);
                exit(1);
            }

            if (tm->trans_count < MAX_TRANSITIONS) {
                tm->transitions[tm->trans_count++] = t;
            }
        }
    }
    fclose(file);

    // Final completeness check
    int required = tm->state_count * tm->tape_count;
    if (tm->trans_count != required) {
        fprintf(stderr, "Error: Transition Table Incomplete!\n");
        fprintf(stderr, "Expected %d rules (%d states * %d tape symbols), but found %d.\n", 
                required, tm->state_count, tm->tape_count, tm->trans_count);
        exit(1);
    }
}