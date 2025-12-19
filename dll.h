#ifndef DLL_H
#define DLL_H

#include <stdio.h>
#include <stdlib.h>

// Node stores a single character
typedef struct Node {
    char data;
    struct Node* next;
    struct Node* prev;
} Node;

// List wrapper
typedef struct {
    Node* head;
} DoublyLinkedList;

// Function Prototypes
DoublyLinkedList* create_list();
void add_node(DoublyLinkedList* list, char data);
void print_forward(DoublyLinkedList* list);
void destroy_list(DoublyLinkedList* list);

#endif