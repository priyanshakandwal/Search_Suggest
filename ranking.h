#ifndef RANKING_H
#define RANKING_H

typedef struct Node {
    char word[100];
    int freq;
    struct Node* next;
} Node;

void initTable();
void addOrUpdateWord(const char* word);
void saveData(const char* filename);
void loadData(const char* filename);
void printTopWords(int N);

#endif