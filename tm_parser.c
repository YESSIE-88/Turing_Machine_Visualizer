#include "tm_parser.h"
#include <string.h>
#include <ctype.h>

void parse_tm_file(const char *filename, TuringMachine *tm) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening TM file");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') continue;

        if (strstr(line, "input_alphabet")) {
            char *p = strchr(line, '{');
            while (p && *p != '}' && tm->input_count < 8) {
                if (isalnum(*p)) tm->input_alphabet[tm->input_count++] = *p;
                p++;
            }
        } 
        else if (strstr(line, "tape_alphabet")) {
            char *p = strchr(line, '{');
            while (p && *p != '}' && tm->tape_count < 9) {
                if (isalnum(*p) || *p == '_') tm->tape_alphabet[tm->tape_count++] = *p;
                p++;
            }
        } 
        else if (strstr(line, "states")) {
            char *p = strchr(line, '{');
            while (p && *p != '}' && tm->state_count < 8) {
                if (*p == 'q') {
                    char next = *(p + 1);
                    // Ignore accept (a) and reject (R/r) specifically
                    if (next != ' ' && next != '}') {
                        tm->states[tm->state_count++] = next;
                    }
                }
                p++;
            }
            break; // Stop reading after states
        }
    }
    fclose(file);
}