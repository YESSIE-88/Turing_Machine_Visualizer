#include "dll.h"

DoublyLinkedList* create_list() {
    DoublyLinkedList* list = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
    if (list) {
        list->head = NULL;
    }
    return list;
}

void add_node(DoublyLinkedList* list, char data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) return;

    newNode->data = data;
    newNode->next = NULL;

    if (list->head == NULL) {
        newNode->prev = NULL;
        list->head = newNode;
        return;
    }

    Node* temp = list->head;
    while (temp->next != NULL) {
        temp = temp->next;
    }

    temp->next = newNode;
    newNode->prev = temp;
}

void print_forward(DoublyLinkedList* list) {
    Node* temp = list->head;
    printf("List Content: ");
    while (temp != NULL) {
        printf("%c ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

void destroy_list(DoublyLinkedList* list) {
    Node* current = list->head;
    Node* nextNode;

    while (current != NULL) {
        nextNode = current->next;
        free(current); // Freeing each node "object"
        current = nextNode;
    }

    free(list); // Freeing the list container
}