#include "dll.h"

int main(int argc, char* argv[]) {
    // Check if the user actually provided an argument
    if (argc < 2) {
        printf("Usage: %s <string>\n", argv[0]);
        return 1;
    }

    DoublyLinkedList* list = create_list();

    // Get the string from the first argument
    char* input = argv[1];

    // Loop through each character until we hit the end of the string
    for (int i = 0; input[i] != '\0'; i++) {
        add_node(list, input[i]);
    }

    // Add the additional underscore at the very end
    add_node(list, '_');

    // Display the result
    print_forward(list);

    // Clean up memory
    destroy_list(list);

    return 0;
}