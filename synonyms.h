#ifndef SYNONYMS_H
#define SYNONYMS_H

#define MAX_WORD_LEN 50

typedef struct Synonym {
    char word[MAX_WORD_LEN];
    struct Synonym *next;
} Synonym;

typedef struct SynonymGroup {
    char mainWord[MAX_WORD_LEN];
    Synonym *list;
    struct SynonymGroup *next;
} SynonymGroup;


SynonymGroup *createGroup(const char *mainWord);
void addSynonym(SynonymGroup **head, const char *mainWord, const char *synWord);
void displaySynonyms(SynonymGroup *head, const char *mainWord);
int areSynonyms(SynonymGroup *head, const char *word1, const char *word2);
void freeAll(SynonymGroup *head);

#endif