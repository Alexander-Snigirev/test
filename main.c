#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100

typedef struct Node {
    char* key;
    char* value;
    struct Node* next;
} Node;

typedef struct HashTable {
    Node* table[SIZE];
} HashTable;

unsigned int hash(char* key) {
    unsigned int hash = 0;
    while (*key) {
        hash = hash * 31 + *key;
        key++;
    }
    return hash % SIZE;
}

HashTable* createHashTable() {
    HashTable* hashtable = (HashTable*)malloc(sizeof(HashTable));
    for (int i = 0; i < SIZE; i++) {
        hashtable->table[i] = NULL;
    }
    return hashtable;
}

void insert(HashTable* hashtable, char* key, char* value) {
    unsigned int index = hash(key);
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->key = strdup(key);
    new_node->value = strdup(value);
    new_node->next = NULL;

    if (hashtable->table[index] == NULL) {
        hashtable->table[index] = new_node;
    } else {
        Node* current = hashtable->table[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

char* lookup(HashTable* hashtable, char* key) {
    unsigned int index = hash(key);
    Node* current = hashtable->table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

int main() {
    HashTable* hashtable = createHashTable();

    insert(hashtable, "name", "Alice");
    insert(hashtable, "age", "25");
    insert(hashtable, "city", "New York");

    printf("Name: %s\n", lookup(hashtable, "name"));
    printf("Age: %s\n", lookup(hashtable, "age"));
    printf("City: %s\n", lookup(hashtable, "city"));

    return 0;
}
