#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ranking.h"   // make sure this header exists and matches your struct declaration

#define TABLE_SIZE 1000

// Global hash table
Node* table[TABLE_SIZE];

// Hash function
unsigned int hash(const char* word) {
    unsigned int hash = 0;
    while (*word) {
        hash = (hash * 31) + *word++;
    }
    return hash % TABLE_SIZE;
}

// Initialize hash table
void initTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = NULL;
    }
}

// Add or update a word frequency
void addOrUpdateWord(const char* word) {
    unsigned int idx = hash(word);
    Node* curr = table[idx];

    while (curr != NULL) {
        if (strcmp(curr->word, word) == 0) {
            curr->freq++;
            return;
        }
        curr = curr->next;
    }

    // Create new node
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->word, word);
    newNode->freq = 1;
    newNode->next = table[idx];
    table[idx] = newNode;
}

// Save data to file
void saveData(const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        perror("Error opening file for writing");
        return;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* curr = table[i];
        while (curr) {
            fprintf(fp, "%s %d\n", curr->word, curr->freq);
            curr = curr->next;
        }
    }
    fclose(fp);
}

// Load data from file
void loadData(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening file for reading");
        return;
    }

    char w[100];
    int f;
    while (fscanf(fp, "%s %d", w, &f) == 2) {
        unsigned int idx = hash(w);
        Node* n = (Node*)malloc(sizeof(Node));
        strcpy(n->word, w);
        n->freq = f;
        n->next = table[idx];
        table[idx] = n;
    }
    fclose(fp);
}

// Comparison for sorting (descending by freq)
int cmp(const void* a, const void* b) {
    Node* x = (Node*)a;
    Node* y = (Node*)b;
    return y->freq - x->freq;
}

// Print top N ranked words
void printTopWords(int N) {
    Node* nodes[10000];
    int count = 0;

    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* curr = table[i];
        while (curr) {
            nodes[count++] = curr;
            curr = curr->next;
        }
    }

    qsort(nodes, count, sizeof(Node*), cmp);

    printf("\nTop %d Words:\n", N);
    for (int i = 0; i < N && i < count; i++) {
        printf("%s — %d\n", nodes[i]->word, nodes[i]->freq);
    }
}